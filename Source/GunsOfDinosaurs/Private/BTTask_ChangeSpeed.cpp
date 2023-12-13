// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_ChangeSpeed.h"

#include "God_AI_Controller.h"
#include "God_Alien.h"
#include "BehaviorTree/BlackboardComponent.h"

/*
====================
UBTTask_ChangeSpeed::UBTTask_ChangeSpeed
====================
*/
UBTTask_ChangeSpeed::UBTTask_ChangeSpeed() {
	NodeName = "Change Speed";	
}

/*
====================
UBTTask_ChangeSpeed::ExecuteTask
====================
*/
EBTNodeResult::Type UBTTask_ChangeSpeed::ExecuteTask( UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory ) {
	const auto AIC = Cast< AGod_AI_Controller >( OwnerComp.GetAIOwner() );
	if ( !AIC ) {
		return EBTNodeResult::Failed;
	}
	if (const auto * const Cont = Cast< AGod_AI_Controller >( OwnerComp.GetAIOwner() ) ) {
		if (const auto * const Alien = Cast< AGod_Alien >( Cont->GetPawn() ) ) {
			if ( auto const B = OwnerComp.GetBlackboardComponent() ) {
				if (const bool bShouldChase = B->GetValueAsBool("CanSeePlayer") ) {
					//Alien->GetCharacterMovement()->MaxWalkSpeed = ChaseSpeed;
					AIC->UpdateSpeed(NodeSpeed);
					FinishLatentTask( OwnerComp, EBTNodeResult::Succeeded);
					return EBTNodeResult::Succeeded;
				} else if ( !bShouldChase ) {
					//Alien->GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
					AIC->UpdateSpeed(NodeSpeed);
					FinishLatentTask( OwnerComp, EBTNodeResult::Succeeded);
					return EBTNodeResult::Succeeded;
				}
				if ( const bool bDistract = B->GetValueAsBool( "DistractionActive" ) ) {
					//Alien->GetCharacterMovement()->MaxWalkSpeed = DistractionSpeed;
					AIC->UpdateSpeed(NodeSpeed);
					FinishLatentTask( OwnerComp, EBTNodeResult::Succeeded);
					return EBTNodeResult::Succeeded;
				}
			}
		}
	}

	return EBTNodeResult::Failed;
}
