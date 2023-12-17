// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_ChangeSpeed.generated.h"

/**
 * 
 */
UCLASS()
class GUNSOFDINOSAURS_API UBTTask_ChangeSpeed : public UBTTask_BlackboardBase
{
public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	GENERATED_BODY()

public:
	UBTTask_ChangeSpeed();

private:
	UPROPERTY( EditAnywhere, BlueprintReadWrite, meta = ( AllowPrivateAccess = "true" ) )
	float NodeSpeed = 560.0f;

};
