// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Director.generated.h"

class AGod_AI_Controller;
class AGodCharacter;
class AGod_Alien;
class AAI_MonsterController;
class UNavigationSystemV1;

UCLASS()
class GUNSOFDINOSAURS_API ADirector : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere)
	float MenaceGauge;
	
	ADirector();
	void ChangeMenaceGauge(float DeltaMenace);
	void GiveNewTask(); 
	ADirector::Super* GivePlayerAlien() const; 
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY()
	AGodCharacter* PlayerCharacter;
	UPROPERTY(EditAnywhere) // Alien Character must be set in editor
	AGod_Alien* AlienCharacter;
	UPROPERTY()
	AGod_AI_Controller* AlienAI;
	UPROPERTY()
	UNavigationSystemV1* NavSys;

	virtual void BeginPlay() override;
	
private:
	FNavLocation TempLocation;

	void BackOff();
};
