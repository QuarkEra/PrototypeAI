// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "God_AI_Controller.generated.h"

/**
 * 
 */

class AGod_Alien;
class AGodCharacter;
class ADirector;

UCLASS()
class GUNSOFDINOSAURS_API AGod_AI_Controller : public AAIController
{
	GENERATED_BODY()

public:
	explicit AGod_AI_Controller( FObjectInitializer const& ObjectInitializer );
	void UpdateSpeed( float NewSpeed );
	void ReceiveNewDirector( ADirector* NewDirector );
	
protected:
	virtual void BeginPlay() override;
	virtual void OnPossess( APawn* InPawn ) override;
	void SetupSightConfig();
	void SetupHearingConfig();

private:
	bool								bPlayerKilled;
	UPROPERTY()
	ADirector*							MyDirector;
	FTimerHandle						SpeedIncreaseTimerHandle;
	float								StartTime;
	UPROPERTY()
	class UAISenseConfig_Sight*			SightConfig;
	UPROPERTY()
	class UAISenseConfig_Hearing*		HearingConfig;
	UPROPERTY()
	AGodCharacter *						ThePlayer;
	UPROPERTY()
	AGod_Alien*							MyPawn;
	
	void SetupPerceptionSystem();
	UFUNCTION()
	void OnTargetDetected( AActor* Actor, FAIStimulus const Stimulus );
	float InKillDistance(const AActor* KillActor) const;
	void Tick(float DeltaSeconds) override;
};
