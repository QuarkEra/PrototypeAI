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
	if ( auto * const MyPawn = Cast< AGod_Alien >(GetPawn() ) ) {
		if ( auto * MC = MyPawn->GetCharacterMovement() ) {
			//const float Alpha = FMath::Clamp(  GetWorld()->DeltaTimeSeconds, 0.0f, 1.0f );
			//const float CurrentSpeed = MC->MaxWalkSpeed;
			//MC->MaxWalkSpeed = FMath::Lerp( CurrentSpeed, NewSpeed, Alpha );

			float StartSpeed = MC->MaxWalkSpeed;

			/*
			 * The following Lambda was used with [=]() which gave a compiler error for C++20
			 * Upon being told of this JetBrains AI Assistant provided the corrected format, wow
			 */
			// Initialize the timer
			FTimerDelegate TimerDel;
			TimerDel.BindLambda([this, StartSpeed, NewSpeed, SpeedIncreaseTime = 0.5f, MC]()
			{
				// Calculate elapsed time and alpha
				float ElapsedTime = GetWorld()->GetTimeSeconds() - StartTime;
				float Alpha = FMath::Clamp(ElapsedTime / SpeedIncreaseTime, 0.f, 1.f);

				// Interpolate the speed using Lerp
				float FinalSpeed = FMath::Lerp(StartSpeed, NewSpeed, Alpha);
				MC->MaxWalkSpeed = FinalSpeed;

				// Stop the timer when reached the target
				if (Alpha >= 1.f)
				{
					GetWorldTimerManager().ClearTimer(SpeedIncreaseTimerHandle);
				}
			});

			// Store the start time and start the timer
			StartTime = GetWorld()->GetTimeSeconds();
			GetWorldTimerManager().SetTimer(SpeedIncreaseTimerHandle, TimerDel, 0.01f, true);
		}
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
	SightConfig->SightRadius = 500.0f;
	SightConfig->LoseSightRadius = SightConfig->SightRadius + 50.0f;
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
	HearingConfig->HearingRange = 1500.0f;
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
	if ( auto * const StimActor = Cast< AGodCharacter >( Actor ) ) {
		bIsPlayer = StimActor->IsPlayerControlled();
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
