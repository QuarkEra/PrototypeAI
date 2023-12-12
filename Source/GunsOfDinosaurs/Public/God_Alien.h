// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "God_PatrolPath.h"
#include "BehaviorTree/BehaviorTree.h"
#include "GameFramework/Character.h"
#include "God_Alien.generated.h"

UCLASS()
class GUNSOFDINOSAURS_API AGod_Alien : public ACharacter
{
	GENERATED_BODY()

public:
	UBehaviorTree* GetBehaviourTree();
	AGod_PatrolPath* GetPatrolPath() const;
	AGod_Alien();
	virtual void Tick( float DeltaTime ) override;
	virtual void SetupPlayerInputComponent( class UInputComponent* PlayerInputComponent ) override;
	float GetChaseSpeed() const;
    float GetWalkSpeed() const;
	
private:
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category="AI", meta=( AllowPrivateAccess="true" ) )
	UBehaviorTree* Tree;
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category="AI", meta=( AllowPrivateAccess="true" ) )
	AGod_PatrolPath* PatrolPath;
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category="AI", meta=( AllowPrivateAccess="true" ) )
	float ChaseSpeed;
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category="AI", meta=( AllowPrivateAccess="true" ) )
	float WalkSpeed;
	
	virtual void BeginPlay() override;

};
