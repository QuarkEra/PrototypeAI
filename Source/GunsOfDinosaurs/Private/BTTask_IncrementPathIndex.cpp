// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_IncrementPathIndex.h"

#include "God_AI_Controller.h"
#include "God_Alien.h"
#include "BehaviorTree/BlackboardComponent.h"

/*
====================
UBTTask_IncrementPathIndex::UBTTask_IncrementPathIndex
====================
*/
UBTTask_IncrementPathIndex::UBTTask_IncrementPathIndex() {
	NodeName = "Increment Path Index";
}

/*
====================
UBTTask_IncrementPathIndex::ExecuteTask
====================
*/
EBTNodeResult::Type UBTTask_IncrementPathIndex::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {
	if ( auto * const Cont = Cast< AGod_AI_Controller >( OwnerComp.GetAIOwner() ) ) {
		if ( auto * const Alien = Cast< AGod_Alien >( Cont->GetPawn() ) ) {
			if ( auto * const BC = OwnerComp.GetBlackboardComponent() ) {
				auto const NoOfPoints = Alien->GetPatrolPath()->Num();
				constexpr auto MinIndex = 0;
				auto const MaxIndex = NoOfPoints - 1;
				auto Index = BC->GetValueAsInt( GetSelectedBlackboardKey() );

				if ( bBiDirectional ) {
					if ( Index >= MaxIndex && Direction == EDirection_t::Forward) {
						Direction = EDirection_t::Reverse;
					} else if ( Index == MinIndex && Direction == EDirection_t::Reverse) {
						Direction = EDirection_t::Forward;
					}
				}

				BC->SetValueAsInt(
					GetSelectedBlackboardKey(),
					(Direction == EDirection_t::Forward ? ++Index : --Index ) % NoOfPoints
					);

				FinishLatentTask( OwnerComp, EBTNodeResult::Succeeded);
				return EBTNodeResult::Succeeded;
			}
		}
	}

	return EBTNodeResult::Failed;
}


