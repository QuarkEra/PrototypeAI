// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_FindPathPoint.h"

#include "God_AI_Controller.h"
#include "God_Alien.h"
#include "BehaviorTree/BlackboardComponent.h"

/*
====================
UBTTask_FindPathPoint::UBTTask_FindPathPoint
====================
*/
UBTTask_FindPathPoint::UBTTask_FindPathPoint() {
	NodeName = "Find Path Point";
}

/*
====================
UBTTask_FindPathPoint::ExecuteTask
====================
*/
EBTNodeResult::Type UBTTask_FindPathPoint::ExecuteTask( UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory ) {
	if (const AGod_AI_Controller* const Cont = Cast<AGod_AI_Controller>(OwnerComp.GetAIOwner()))	 {
		if (UBlackboardComponent* const BC = OwnerComp.GetBlackboardComponent()) {
			int32 const Index = BC->GetValueAsInt(GetSelectedBlackboardKey());
			if (const AGod_Alien* Alien = Cast<AGod_Alien>(Cont->GetPawn())) {
				FVector const Point = Alien->GetPatrolPath()->GetPatrolPoint(Index);
				UE::Math::TVector<double> const GlobalPoint = Alien->GetPatrolPath()->GetActorTransform().
				                                                     TransformPosition(Point);
				BC->SetValueAsVector( PatrolPathVectorKey.SelectedKeyName, GlobalPoint );
				FinishLatentTask( OwnerComp, EBTNodeResult::Succeeded );
				return EBTNodeResult::Succeeded;
			}
		}
	}
	
	return EBTNodeResult::Failed;
}
