// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GodCharacter.h"
#include "GameFramework/Actor.h"
#include "GodWeapon.generated.h"

class UGodInventoryComponent;

UENUM(BlueprintType)
enum class EWeaponClass : uint8
{
	Melee,
	Pistol,
	Shotgun,
	Bow,
};

UENUM(BlueprintType)
enum class EAmmoType : uint8
{
	Primary,
	Secondary,
};

UCLASS()
class GUNSOFDINOSAURS_API AGodWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGodWeapon();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bIsActiveWeapon;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Gameplay")
	bool bIsCollected;
	bool bHasAmmo;
	[[nodiscard]] bool IsHasAmmo() const;
	__declspec(property(get = IsHasAmmo)) bool FHasAmmo;

	UPROPERTY(BlueprintReadWrite, Category=Weapon)
	EWeaponClass WeaponClass;
	
	UFUNCTION(BlueprintCallable, Category="Weapon")
	void Shoot();
	void CycleAmmoType();

	UPROPERTY(EditAnywhere, Category=Gameplay)
	USoundBase* ShootSound;
	UPROPERTY(EditAnywhere, Category=Gameplay)
	USoundBase* SecondaryAmmoShootSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	bool bIsHitScanWeapon = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	FName MuzzleName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	UParticleSystem* ImpactFleshEffect;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	UParticleSystem* ImpactDefaultEffect;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	UParticleSystem* ImpactSecondaryAmmoEffect;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	bool HandleCharacterAmmo(AGodCharacter* Character);
	void SoundSelectionByAmmoType(const AGodCharacter* Character) const;
	int GetAvailableAmmo(EWeaponClass MyClass, EAmmoType AmmoType) const;
	void ExpendAmmo(EWeaponClass AmmoClass, EAmmoType AmmoType);
	void DoHitScan(FHitResult& Hit);
	void DoPhysicalSurfaceGetFromHitScan(FHitResult Hit, UParticleSystem*& SelectedEffect);

	UPROPERTY(EditAnywhere)
	TSubclassOf<UActorComponent> CompClass;

	UPROPERTY(VisibleAnywhere)
	EAmmoType CurrentAmmoType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UDamageType> DamageType;


};
