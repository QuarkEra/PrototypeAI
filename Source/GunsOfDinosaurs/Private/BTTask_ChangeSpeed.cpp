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
	if (const AGod_AI_Controller* const Cont = Cast<AGod_AI_Controller>(OwnerComp.GetAIOwner())) {
		if (const AGod_Alien* const Alien = Cast<AGod_Alien>(Cont->GetPawn())) {
			if (UBlackboardComponent const* B = OwnerComp.GetBlackboardComponent()) {
				if ( const bool bShouldChase = B->GetValueAsBool( "CanSeePlayer" ) ) {
					AIC->UpdateSpeed( NodeSpeed );
					FinishLatentTask( OwnerComp, EBTNodeResult::Succeeded);
					return EBTNodeResult::Succeeded;
				} else if ( !bShouldChase ) {
					AIC->UpdateSpeed( NodeSpeed );
					FinishLatentTask( OwnerComp, EBTNodeResult::Succeeded);
					return EBTNodeResult::Succeeded;
				}
				if ( const bool bDistract = B->GetValueAsBool( "DistractionActive" ) ) {
					AIC->UpdateSpeed( NodeSpeed );
					FinishLatentTask( OwnerComp, EBTNodeResult::Succeeded);
					return EBTNodeResult::Succeeded;
				}
			}
		}
	}

	return EBTNodeResult::Failed;
}
