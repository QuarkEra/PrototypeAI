// Fill out your copyright notice in the Description page of Project Settings.


#include "God_AI_Controller.h"

#include "GodCharacter.h"
#include "God_Alien.h"
#include "BehaviorTree/BlackboardComponent.h"
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
	bool const bIsPlayer =  Cast< AGodCharacter >( Actor )->IsPlayerControlled();
	if ( bIsPlayer && Stimulus.Type.Name == "Default__AISense_Sight") {
		GetBlackboardComponent()->SetValueAsBool( "CanSeePlayer", Stimulus.WasSuccessfullySensed() );		
	}
	const FName DistractionTag = Stimulus.Tag;
	if (  Stimulus.Tag == DistractionTag && Stimulus.Type.Name == "Default__AISense_Hearing" ) {
		GetBlackboardComponent()->SetValueAsVector( "TargetLocation", Stimulus.StimulusLocation );
		GetBlackboardComponent()->SetValueAsBool( "DistractionActive", Stimulus.WasSuccessfullySensed() );
	}
}

