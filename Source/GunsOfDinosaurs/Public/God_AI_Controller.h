// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "God_AI_Controller.generated.h"

/**
 * 
 */
UCLASS()
class GUNSOFDINOSAURS_API AGod_AI_Controller : public AAIController
{
	GENERATED_BODY()

public:
	explicit AGod_AI_Controller(FObjectInitializer const& ObjectInitializer);


protected:
	virtual void BeginPlay() override;

	/**
	 * 
	 */

	virtual void OnPossess(APawn* InPawn) override;
	
};