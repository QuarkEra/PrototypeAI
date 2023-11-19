// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GodGameInstance.generated.h"

enum class EAmmoType : uint8;
enum class EWeaponClass : uint8;
/**
 * 
 */
UCLASS()
class GUNSOFDINOSAURS_API UGodGameInstance : public UGameInstance
{
	GENERATED_BODY()
	UGodGameInstance();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int PistolAmmo;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int ShotgunPrimaryAmmo;
	int ShotgunSecondaryAmmo;

public:
	int GetAmmo(EWeaponClass WeaponClass, EAmmoType AmmoType) const;
	// This add function is used in blueprints for pickups to make it quicker to try new pickups
	UFUNCTION(BlueprintCallable)
	void AddAmmo(EWeaponClass WeaponClass, int AmmoCount, EAmmoType AmmoType);
	void UseAmmo(EWeaponClass WeaponClass, EAmmoType AmmoType);
	
};
