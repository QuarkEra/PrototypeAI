// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GodCharacter.h"
#include "Components/SkeletalMeshComponent.h"
#include "GodWeaponComponent.generated.h"

/**
 * 
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GUNSOFDINOSAURS_API UGodWeaponComponent : public USkeletalMeshComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category=Input)
	class UInputMappingContext* ShootMappingContext;
	UPROPERTY(EditAnywhere, Category=Input)
	class UInputAction* ShootAction;

	UPROPERTY(EditAnywhere, Category=Gameplay)
	USoundBase* ShootSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	UAnimationAsset* ShootAnimation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	bool bIsHitScanWeapon = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	FName MuzzleName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	UParticleSystem* ImpactFleshEffect;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	UParticleSystem* ImpactDefaultEffect;
	
	UPROPERTY(EditDefaultsOnly, Category=Projectile)
	TSubclassOf<class AGodProjectile> ProjectileClass;

	

	UFUNCTION(BlueprintCallable, Category="Weapon")
	void AttachWeapon(AGodCharacter* TargetCharacter);

	UFUNCTION(BlueprintCallable, Category="Weapon")
	void Shoot();

	UGodWeaponComponent();

protected:
	UFUNCTION()
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	UPROPERTY()
	AGodCharacter* Character;
};
