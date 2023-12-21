// Fill out your copyright notice in the Description page of Project Settings.


#include "God_AI_Controller.h"

#include "GodCharacter.h"
#include "God_Alien.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AISenseConfig_Sight.h"

/*
====================
AGod_AI_Controller::AGod_AI_Controller
====================
*/
AGod_AI_Controller::AGod_AI_Controller( FObjectInitializer const& ObjectInitializer )
{
	SetupPerceptionSystem();
}

/*
====================
AGod_AI_Controller::UpdateSpeed
====================
*/
void AGod_AI_Controller::UpdateSpeed(float NewSpeed) {
	if ( const AGod_Alien * const AlienPawn = Cast< AGod_Alien >(GetPawn() ) ) {
		if ( UCharacterMovementComponent* MC = AlienPawn->GetCharacterMovement() ) {
			float StartSpeed = MC->MaxWalkSpeed;

			/*
			 * The following Lambda was used with [=]() which gave a compiler error for C++20
			 * Upon being told of this JetBrains AI Assistant provided the corrected format, wow
			 */
			FTimerDelegate TimerDel;
			TimerDel.BindLambda( [ this, StartSpeed, NewSpeed, SpeedIncreaseTime = 0.5f, MC ]()
			{
				const float ElapsedTime = GetWorld()->GetTimeSeconds() - StartTime;
				const float Alpha = FMath::Clamp( ElapsedTime / SpeedIncreaseTime, 0.f, 1.f );
				const float FinalSpeed = FMath::Lerp( StartSpeed, NewSpeed, Alpha );
				MC->MaxWalkSpeed = FinalSpeed;
				if (Alpha >= 1.f)
				{
					GetWorldTimerManager().ClearTimer( SpeedIncreaseTimerHandle );
				}
			} );
			
			StartTime = GetWorld()->GetTimeSeconds();
			GetWorldTimerManager().SetTimer( SpeedIncreaseTimerHandle, TimerDel, 0.01f, true );
		}
	}
}

/*
====================
AGod_AI_Controller::ReceiveNewDirector
====================
*/
void AGod_AI_Controller::ReceiveNewDirector( ADirector* NewDirector ) {
	if ( ensure( NewDirector != nullptr ) ) {
		this->MyDirector = NewDirector;
	}
}

/*
====================
AGod_AI_Controller::BeginPlay
====================
*/
void AGod_AI_Controller::BeginPlay()
{
	Super::BeginPlay();
	
}

/*
====================
AGod_AI_Controller::OnPossess
====================
*/
void AGod_AI_Controller::OnPossess( APawn* InPawn )
{
	Super::OnPossess( InPawn );
	
	MyPawn = Cast< AGod_Alien >( InPawn );
	if ( AGod_Alien* const Alien = Cast< AGod_Alien >( InPawn ) )
	{
		if ( UBehaviorTree* const Tree = Alien->GetBehaviourTree() )
		{
			UBlackboardComponent* b;
			UseBlackboard( Tree->BlackboardAsset, b );
			Blackboard = b;
			RunBehaviorTree( Tree );
		}
	}
}

/*
====================
AGod_AI_Controller::SetupSightConfig
====================
*/
void AGod_AI_Controller::SetupSightConfig() {
	SetPerceptionComponent( *CreateDefaultSubobject< UAIPerceptionComponent >( TEXT ( "Perception Component" ) ) );
	SightConfig->SightRadius = 1500.0f;
	SightConfig->LoseSightRadius = SightConfig->SightRadius + 225.0f;
	SightConfig->PeripheralVisionAngleDegrees = 90.0f;
	SightConfig->SetMaxAge( 5.0f );
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

	GetPerceptionComponent()->SetDominantSense( *SightConfig->GetSenseImplementation() );
	GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic( this, &AGod_AI_Controller::OnTargetDetected );
	GetPerceptionComponent()->ConfigureSense( ( *SightConfig ) );
}

/*
====================
AGod_AI_Controller::SetupHearingConfig
====================
*/
void AGod_AI_Controller::SetupHearingConfig() {
	HearingConfig->HearingRange = 2000.0f;
	HearingConfig->SetMaxAge( 5.0f );
	HearingConfig->DetectionByAffiliation.bDetectEnemies = true;
	HearingConfig->DetectionByAffiliation.bDetectFriendlies = true;
	HearingConfig->DetectionByAffiliation.bDetectNeutrals = true;

	GetPerceptionComponent()->ConfigureSense( ( *HearingConfig ) );
}

/*
====================
AGod_AI_Controller::SetupPerceptionSystem
====================
*/
void AGod_AI_Controller::SetupPerceptionSystem() {
	SightConfig = CreateDefaultSubobject< UAISenseConfig_Sight >( TEXT ( "Sight Config" ) );
	if ( SightConfig ) {
		SetupSightConfig();
	}
	HearingConfig = CreateDefaultSubobject< UAISenseConfig_Hearing >( TEXT( "Hearing Config" ) );
	if ( HearingConfig ) {
		SetupHearingConfig();
	}
}

/*
====================
AGod_AI_Controller::OnTargetDetected
====================
*/
void AGod_AI_Controller::OnTargetDetected( AActor* Actor, FAIStimulus const Stimulus ) {
	
	bool bIsPlayer = false;
	ThePlayer = Cast< AGodCharacter >( Actor );
	if ( ThePlayer ) {
		bIsPlayer = ThePlayer->IsPlayerControlled();
	}
	if ( bIsPlayer && Stimulus.Type.Name == "Default__AISense_Sight") {
		GetBlackboardComponent()->SetValueAsBool( "CanSeePlayer", Stimulus.WasSuccessfullySensed() );
	}
	
	const FName DistractionTag = Stimulus.Tag;
	if (  Stimulus.Tag == DistractionTag && Stimulus.Type.Name == "Default__AISense_Hearing" ) {
		GetBlackboardComponent()->SetValueAsVector( "TargetLocation", Stimulus.StimulusLocation );
		GetBlackboardComponent()->SetValueAsBool( "DistractionActive", Stimulus.WasSuccessfullySensed() );
	}
	
}

/*
====================
AGod_AI_Controller::InKillDistance
====================
*/
float AGod_AI_Controller::InKillDistance( const AActor* KillActor ) const {
	const FVector myLoc = MyPawn->GetActorLocation();
	return KillActor ? ( myLoc - KillActor->GetActorLocation() ).Size() : 0.f;
}

/*
====================
AGod_AI_Controller::Tick
====================
*/
void AGod_AI_Controller::Tick(float DeltaSeconds) {
	Super::Tick(DeltaSeconds);
	
	if ( MyPawn != nullptr ) {
		float Dist = InKillDistance( ThePlayer );
		if ( Dist <= 120.0f  && Dist > 0 && bPlayerKilled == false) {
			StopMovement();
			MyPawn->PlayEatingSound();
			GetBlackboardComponent()->SetValueAsBool( "bPlayerKilled", true );
			bPlayerKilled = true;
			if ( ThePlayer != nullptr) {
				StopMovement();
				ThePlayer->KillPlayer( MyPawn->GetActorLocation() );
			}
		}
	}
}
