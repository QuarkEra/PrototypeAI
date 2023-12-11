// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BehaviorTree.h"
#include "GameFramework/Character.h"
#include "God_Alien.generated.h"

UCLASS()
class GUNSOFDINOSAURS_API AGod_Alien : public ACharacter
{
	GENERATED_BODY()

public:
	AGod_Alien();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	UBehaviorTree* GetBehaviourTree();
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI", meta=(AllowPrivateAccess="true"))
	UBehaviorTree* Tree;
	
	virtual void BeginPlay() override;

};
