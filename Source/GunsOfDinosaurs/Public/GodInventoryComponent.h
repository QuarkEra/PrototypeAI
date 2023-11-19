// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GodInventoryComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GUNSOFDINOSAURS_API UGodInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGodInventoryComponent();

	UPROPERTY(VisibleAnywhere)
	int PistolAmmo;
	UPROPERTY(VisibleAnywhere)
	int ShotgunAmmo;
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
	
};
