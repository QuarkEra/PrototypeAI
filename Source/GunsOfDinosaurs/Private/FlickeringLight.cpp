// Fill out your copyright notice in the Description page of Project Settings.


#include "FlickeringLight.h"

#include "Components/SpotLightComponent.h"
#include "Math/UnrealMathUtility.h"


// Sets default values
AFlickeringLight::AFlickeringLight()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bShouldFlicker = false;
	TimeOn = 0.2f;
	TimeOff = 1.0f;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComp"));
	RootComponent = SceneComponent;
	
	SpotLightComponent = CreateDefaultSubobject<USpotLightComponent>(TEXT("SpotLight"));
	SpotLightComponent->SetupAttachment(SceneComponent);
	SpotLightComponent->SetWorldRotation(FRotator(-90,0,0));
	
}

bool AFlickeringLight::IsShouldFlicker() const
{
	return bShouldFlicker;
}

void AFlickeringLight::SetShouldFlicker(bool bNewShouldFlicker)
{
	this->bShouldFlicker = bNewShouldFlicker;
}

void AFlickeringLight::ReceiveDistance(double VectorDiff, double FlickerRad)
{
	VecDiff = VectorDiff;
	FlickerRadius = FlickerRad;
	FlickerRate = FMath::GetMappedRangeValueClamped(FVector2d(0, FlickerRadius), FVector2d(TimeOn, TimeOff), VecDiff);
	FlickerRate = FMath::Clamp(FlickerRate, TimeOn, TimeOff);
}

void AFlickeringLight::DoFlicker()
{
	GetWorld()->GetTimerManager().SetTimer(FlickerTimerHandle, this, &AFlickeringLight::ToggleLightOff, FlickerRate, false);
}

void AFlickeringLight::ToggleLightOn()
{
	SpotLightComponent->SetVisibility(true);
}

void AFlickeringLight::ToggleLightOff()
{
	SpotLightComponent->SetVisibility(false);
	
	GetWorld()->GetTimerManager().SetTimer(DelayHandle, this, &AFlickeringLight::ToggleLightOn, TimeOn, false);
	bIsFlickering = false;
}

// Called every frame
void AFlickeringLight::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bShouldFlicker && !bIsFlickering)
	{
		bIsFlickering = true;
		DoFlicker();
	}
}

