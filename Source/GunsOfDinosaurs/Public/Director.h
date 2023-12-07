// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Director.generated.h"

class AGodCharacter;
class AMonsterCharacter;
class AAI_MonsterController;
class UNavigationSystemV1;

UCLASS()
class GUNSOFDINOSAURS_API ADirector : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ADirector();

	void ChangeMenaceGauge(float DeltaMenace);
	float CheckMenaceGauge() const;

	void GiveNewTask();
	ADirector::Super* GiveMonsterActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	AGodCharacter* PlayerCharacter;
	UPROPERTY(EditInstanceOnly)
	AMonsterCharacter* MonsterCharacter;
	UPROPERTY()
	AAI_MonsterController* MonsterAI;

	UPROPERTY()
	UNavigationSystemV1* NavSys;

private:

	FNavLocation TempLocation;

	
	void BackOff();
	// Still undecided on how much control to give Director this early in introduction
	//FNavLocation GetRandomPointsAroundPlayer(APawn* PlayerPawn);

	

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(VisibleAnywhere)
	float MenaceGauge;
};
