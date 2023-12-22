// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_IncrementPathIndex.generated.h"


UCLASS()
class GUNSOFDINOSAURS_API UBTTask_IncrementPathIndex : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	UBTTask_IncrementPathIndex();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	enum class EDirection_t {
		Forward,
		Reverse
	};

	EDirection_t Direction = EDirection_t::Forward;
	
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category="AI", meta=( AllowPrivateAccess="true" ) )
	bool bBiDirectional = false;
	
	
};
