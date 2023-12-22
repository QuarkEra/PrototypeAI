// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_ChangeSpeed.h"

#include "God_AI_Controller.h"
#include "God_Alien.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

/*
====================
UBTService_ChangeSpeed::UBTService_ChangeSpeed
====================
*/
UBTService_ChangeSpeed::UBTService_ChangeSpeed()
{
	NodeName = "Change Speed";

	bNotifyBecomeRelevant = true;
	bNotifyCeaseRelevant = false;
}

/*
====================
UBTService_ChangeSpeed::OnBecomeRelevant
====================
*/
void UBTService_ChangeSpeed::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);
	
	if ( const AGod_AI_Controller* const Cont = Cast< AGod_AI_Controller >( OwnerComp.GetAIOwner() ) ) {
		if ( const AGod_Alien* const Alien = Cast< AGod_Alien >( Cont->GetPawn() ) ) {
			if ( UBlackboardComponent const* B = OwnerComp.GetBlackboardComponent() ) {
				const float Alpha = FMath::Clamp( ( UGameplayStatics::GetWorldDeltaSeconds( GetWorld() ) / 1.0f ), 0.0f, 1.0f );
				const float CurrentSpeed = Alien->GetCharacterMovement()->MaxWalkSpeed;
				if ( const bool bShouldChase = B->GetValueAsBool( "CanSeePlayer") ) {
					//Alien->GetCharacterMovement()->MaxWalkSpeed = ChaseSpeed;
					Alien->GetCharacterMovement()->MaxWalkSpeed = FMath::Lerp( CurrentSpeed, ChaseSpeed, Alpha );
				} else if ( !bShouldChase ) {
					//Alien->GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
					Alien->GetCharacterMovement()->MaxWalkSpeed = FMath::Lerp( CurrentSpeed, WalkSpeed, Alpha );
				}
				if ( const bool bDistract = B->GetValueAsBool( "DistractionActive" ) ) {
					//Alien->GetCharacterMovement()->MaxWalkSpeed = DistractionSpeed;
					Alien->GetCharacterMovement()->MaxWalkSpeed = FMath::Lerp( CurrentSpeed, DistractionSpeed, Alpha );
				}
			}
		}
	}
}
