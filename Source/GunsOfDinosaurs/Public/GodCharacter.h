// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GodCharacter.generated.h"

class ADirector;
class AMonsterCharacter;
class UPawnSensingComponent;
class UGodInventoryComponent;
class AGodWeapon;
class UCameraComponent;
class UGodWeaponComponent;
struct FInputActionValue;
struct FInputActionInstance;
class UInputAction;
class UInputMappingContext;

UCLASS()
class GUNSOFDINOSAURS_API AGodCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	bool										bCaught;
	UPROPERTY(EditAnywhere, Category=Alien)
	float										LOSMultiplier;
	UPROPERTY(EditAnywhere, Category=Alien, meta=(ClampMin=0, ClampMax=10))
	double										DistanceToAlien;
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Weapon")
	AGodWeapon*									CurrentWeapon;   	

	AGodCharacter();
	void ReceiveNewDirector(ADirector* NewDirector);
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; };
	void KillPlayer(const FVector& CatcherLocation);

protected:
	float										StartTime;
	FTimerHandle								TimerDeathTimerHandle;
	FVector										FaceDeath;
	float DesiredFov							= 35;
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category="Audio", meta=( AllowPrivateAccess="true" ) )
	USoundCue*									DeathSounds;
	UPROPERTY(EditAnywhere, Category="Input")
	float										WalkSpeed;
	UPROPERTY(EditAnywhere, Category="Input")
	float										SprintSpeed;
	UPROPERTY(EditAnywhere, Category="Input")
	float										CrouchSpeed;
	UPROPERTY()
	ADirector*									MyDirector;
	UPROPERTY(BlueprintReadOnly, Category=Components)
	UCameraComponent*							CameraComp;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Components)
	USkeletalMeshComponent*						Mesh1P;
	UPROPERTY()
	UPawnSensingComponent*						SensingComponent;
	UPROPERTY(EditDefaultsOnly,Category=Alien)
	AActor*										AlienCharacter;
	UPROPERTY(EditDefaultsOnly, Category="Input")
	UInputMappingContext*						InputMapping;
	UPROPERTY(EditDefaultsOnly, Category="Input")
	UInputAction*								IA_Move;
	UPROPERTY(EditDefaultsOnly, Category="Input")
	UInputAction*								IA_Look;
	UPROPERTY(EditDefaultsOnly, Category="Input")
	UInputAction*								IA_Sprint;
	UPROPERTY(EditDefaultsOnly, Category="Input")
	UInputAction*								IA_Hold;
	UPROPERTY(EditDefaultsOnly, Category="Input")
	UInputAction*								IA_ShootWeapon;
	UPROPERTY(EditDefaultsOnly, Category="Input")
	UInputAction*								IA_CycleAmmoType;
	UPROPERTY()
	class UAIPerceptionStimuliSourceComponent*	StimulusSource;
	
	virtual void BeginPlay() override;
	void SetupStimulusSource();
	void Move( const FInputActionInstance& Instance );
	void Look( const FInputActionValue& Value );
	void StartSprint();
	void StopSprint();
	UFUNCTION(BlueprintCallable)
	void ShootWeapon();
	void StopShootWeapon();
	void CycleAmmoType();
	void LineOfSightToEnemy();
	void DeathAnimation(const FVector& CatcherLocation, float DeltaTime);
};
