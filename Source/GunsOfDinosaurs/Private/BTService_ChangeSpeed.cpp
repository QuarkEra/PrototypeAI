// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_ChangeSpeed.h"

#include "God_AI_Controller.h"
#include "God_Alien.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

/*
====================
UBTService_ChangeSpeed::UBTService_ChangeSpeed
====================
*/
UBTService_ChangeSpeed::UBTService_ChangeSpeed()
{
	NodeName = "Change Speed";

	bNotifyBecomeRelevant = true;
	bNotifyCeaseRelevant = true;
}

/*
====================
UBTService_ChangeSpeed::OnBecomeRelevant
====================
*/
void UBTService_ChangeSpeed::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);
	
	if (const auto * const Cont = Cast< AGod_AI_Controller >( OwnerComp.GetAIOwner() ) ) {
		if (const auto * const Alien = Cast< AGod_Alien >( Cont->GetPawn() ) ) {
			if ( auto const B = OwnerComp.GetBlackboardComponent() ) {
				if (const bool bShouldChase = B->GetValueAsBool("CanSeePlayer") ) {
					Alien->GetCharacterMovement()->MaxWalkSpeed = ChaseSpeed;
				} else if ( !bShouldChase ) {
					Alien->GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
				}
				if ( const bool bDistract = B->GetValueAsBool( "DistractionActive" ) ) {
					Alien->GetCharacterMovement()->MaxWalkSpeed = DistractionSpeed;
				}
			}
		}
	}
}
