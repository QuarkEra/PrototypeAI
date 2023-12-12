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
	if ( auto const Cont = Cast< AGod_AI_Controller >( OwnerComp.GetAIOwner() ) )	 {
		if ( auto* const BC = OwnerComp.GetBlackboardComponent() ) {
			auto const Index = BC->GetValueAsInt( GetSelectedBlackboardKey() );
			if ( auto * Alien = Cast< AGod_Alien >( Cont->GetPawn() ) ) {
				auto const  Point = Alien->GetPatrolPath()->GetPatrolPoint( Index );
				auto const GlobalPoint = Alien->GetPatrolPath()->GetActorTransform().TransformPosition( Point );
				BC->SetValueAsVector( PatrolPathVectorKey.SelectedKeyName, GlobalPoint );
				FinishLatentTask( OwnerComp, EBTNodeResult::Succeeded );
				return EBTNodeResult::Succeeded;
			}
		}
	}
	
	return EBTNodeResult::Failed;
}
