// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_FindRandomLocation.h"

#include "God_AI_Controller.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"

/*
====================
UBTTask_FindRandomLocation::UBTTask_FindRandomLocation
====================
*/
UBTTask_FindRandomLocation::UBTTask_FindRandomLocation(FObjectInitializer const& ObjectInitializer)
{
		NodeName = "Find Random Location";
}

/*
====================
UBTTask_FindRandomLocation::ExecuteTask
====================
*/
EBTNodeResult::Type UBTTask_FindRandomLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (const AGod_AI_Controller* Controller = Cast<AGod_AI_Controller>(OwnerComp.GetAIOwner()))
	{
		if (const auto Alien = Controller->GetPawn())
		{
			const auto Origin = Alien->GetActorLocation();

			if (const auto* NavSys = UNavigationSystemV1::GetCurrent(GetWorld()))
			{
				FNavLocation Loc;
				if (NavSys->GetRandomPointInNavigableRadius(Origin, SearchRadius, Loc))
				{
					OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), Loc.Location);
				}

				FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			}
		}
	}
	
	return EBTNodeResult::Failed;
}
