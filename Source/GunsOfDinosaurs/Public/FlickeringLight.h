// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FlickeringLight.generated.h"

class USpotLightComponent;

UCLASS()
class GUNSOFDINOSAURS_API AFlickeringLight : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFlickeringLight();

	UPROPERTY(EditAnywhere)
	bool bShouldFlicker;
	[[nodiscard]] bool IsShouldFlicker() const;
	void SetShouldFlicker(bool bNewShouldFlicker);
	__declspec(property(get = IsShouldFlicker, put = SetShouldFlicker)) bool FShouldFlicker;
	bool bIsFlickering;
	UPROPERTY(EditAnywhere)
	float TimeOff;
	UPROPERTY(EditAnywhere)
	float TimeOn;
	
	void DoFlicker();
	void ToggleLightOn();
	void ToggleLightOff();

	void ReceiveDistance(double VectorDiff, double FlickerRad);
	double VecDiff;
	double FlickerRadius;
	double FlickerRate;

	double LightMaxIntensity;
	double LightIntensity;
	void SetIntensity(double NewIntensity);
	bool ShouldChangeIntensity;

protected:

	UPROPERTY()
	USceneComponent* SceneComponent;
	UPROPERTY(EditAnywhere)
	USpotLightComponent* SpotLightComponent;

	FTimerHandle DelayHandle;
	FTimerHandle FlickerTimerHandle;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
