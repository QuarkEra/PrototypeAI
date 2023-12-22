// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_ChasePlayer.h"

#include "God_AI_Controller.h"
#include "God_Alien.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"

/*
====================
UBTTask_ChasePlayer::UBTTask_ChasePlayer
====================
*/
UBTTask_ChasePlayer::UBTTask_ChasePlayer() {
	NodeName = "Chase Player";
}

/*
====================
UBTTask_ChasePlayer::ExecuteTask
====================
*/
EBTNodeResult::Type UBTTask_ChasePlayer::ExecuteTask( UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory ) {
	if ( AGod_AI_Controller* const Cont = Cast< AGod_AI_Controller >( OwnerComp.GetAIOwner() ) ) {
		FVector const PlayerLocation = OwnerComp.GetBlackboardComponent()->GetValueAsVector( GetSelectedBlackboardKey() );
		UAIBlueprintHelperLibrary::SimpleMoveToLocation( Cont, PlayerLocation );
		FinishLatentTask( OwnerComp, EBTNodeResult::Succeeded );
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}
