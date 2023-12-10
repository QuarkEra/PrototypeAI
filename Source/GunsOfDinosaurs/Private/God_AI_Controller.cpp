// Fill out your copyright notice in the Description page of Project Settings.


#include "God_AI_Controller.h"

#include "God_Alien.h"

AGod_AI_Controller::AGod_AI_Controller(FObjectInitializer const& ObjectInitializer)
{
	
}

void AGod_AI_Controller::BeginPlay()
{
	Super::BeginPlay();
	
}

void AGod_AI_Controller::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (AGod_Alien* const Alien = Cast<AGod_Alien>(InPawn))
	{
		if (UBehaviorTree* const Tree = Alien->GetBehaviourTree())
		{
			UBlackboardComponent* b;
			UseBlackboard(Tree->BlackboardAsset, b);
			Blackboard = b;
			RunBehaviorTree(Tree);
		}
	}

}

