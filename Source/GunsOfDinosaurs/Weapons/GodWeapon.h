// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GodCharacter.h"
#include "GameFramework/Actor.h"
#include "GodWeapon.generated.h"

class UPointLightComponent;
class UGodInventoryComponent;

// this is also used for selecting ammo class 
UENUM(BlueprintType)
enum class EWeaponClass : uint8
{
	Melee,
	Pistol,
	Flame,
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
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Gameplay")
	bool bIsCollected;
	
	bool bHasAmmo;
	[[nodiscard]] bool IsHasAmmo() const;
	void CauseDamageToValidActor(AActor* HitCharacter);
	__declspec(property(get = IsHasAmmo)) bool FHasAmmo;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	EWeaponClass WeaponClass;
	
	UFUNCTION(BlueprintCallable, Category="Weapon")
	void Shoot();
	UFUNCTION(BlueprintCallable, Category="Weapon")
	void StopShoot();
	
	void CycleAmmoType();

	UPROPERTY(EditDefaultsOnly, Category = "Gameplay")
	USoundBase* ShootSound;
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay")
	USoundBase* SecondaryAmmoShootSound;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Gameplay")
	bool bIsHitScanWeapon = false;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Gameplay")
	FName MuzzleName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Gameplay")
	UParticleSystem* ImpactFleshEffect;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Gameplay")
	UParticleSystem* ImpactDefaultEffect;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Gameplay")
	UParticleSystem* ImpactSecondaryAmmoEffect;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Gameplay")
	bool bFlame = false;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Gameplay")
	USceneComponent* SceneComponent;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Gameplay")
	USkeletalMeshComponent* SkeletalMesh;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Gameplay")
	UParticleSystemComponent* MuzzleParticleEffect;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Gameplay")
	UPointLightComponent* PointLightComponent;
	
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
