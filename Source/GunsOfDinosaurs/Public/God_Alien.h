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
	void PlayEatingSound();
	virtual void Tick( float DeltaTime ) override;
	virtual void SetupPlayerInputComponent( class UInputComponent* PlayerInputComponent ) override;
	
private:
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category="AI", meta=( AllowPrivateAccess="true" ) )
	USoundCue*				EatingSound;
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category="AI", meta=( AllowPrivateAccess="true" ) )
	UBehaviorTree*			Tree;
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category="AI", meta=( AllowPrivateAccess="true" ) )
	AGod_PatrolPath*		PatrolPath;
	
	virtual void BeginPlay() override;

};
