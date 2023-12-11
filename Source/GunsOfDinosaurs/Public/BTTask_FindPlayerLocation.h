// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_FindPlayerLocation.generated.h"

/**
 * 
 */
UCLASS()
class GUNSOFDINOSAURS_API UBTTask_FindPlayerLocation : public UBTTask_BlackboardBase
{
	UBTTask_FindPlayerLocation();

public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	GENERATED_BODY()

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Search", meta = ( AllowPrivateAccess = "true" ) )
	bool bSearchRandom = false;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Search", meta = ( AllowPrivateAccess = "true" ) )
	float Radius = 1000.0f;
};
