// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "GodPickupComponent.generated.h"


class AGodCharacter;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPickUp, AGodCharacter*, PickUpCharacter);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GUNSOFDINOSAURS_API UGodPickupComponent : public USphereComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UGodPickupComponent();

	UPROPERTY(BlueprintAssignable, Category = "Interaction")
	FOnPickUp OnPickUp;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:

	UFUNCTION()
	void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
