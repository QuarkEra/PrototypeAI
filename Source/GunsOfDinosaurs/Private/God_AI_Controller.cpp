// Fill out your copyright notice in the Description page of Project Settings.


#include "God_AI_Controller.h"

#include "GodCharacter.h"
#include "God_Alien.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"
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
AGod_AI_Controller::SetupPerceptionSystem
====================
*/
void AGod_AI_Controller::SetupPerceptionSystem() {
	SightConfig = CreateDefaultSubobject< UAISenseConfig_Sight >( TEXT ( "Sight Config" ) );
	if ( SightConfig ) {
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
}

/*
====================
AGod_AI_Controller::OnTargetDetected
====================
*/
void AGod_AI_Controller::OnTargetDetected( AActor* Actor, FAIStimulus const Stimulus ) {
	if ( auto* const ch =  Cast< AGodCharacter >( Actor ) ) {
		GetBlackboardComponent()->SetValueAsBool( "CanSeePlayer", Stimulus.WasSuccessfullySensed() );		
	}
}

