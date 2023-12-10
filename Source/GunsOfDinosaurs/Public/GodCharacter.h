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
	// Sets default values for this character's properties
	AGodCharacter();
	void ReceiveNewDirector(ADirector* NewDirector);

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; };

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Weapon")
	AGodWeapon* CurrentWeapon;

	void CharacterCaught(const FVector& CatcherLocation);

	UPROPERTY(EditAnywhere, Category=Monster)
	float LOSToMonsterMgMultiplier;
	UPROPERTY(EditAnywhere, Category=Monster, meta=(ClampMin=0, ClampMax=10))
	double DistToMonsterMgMultiplier;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY()
	ADirector* MyDirector;
	
	UPROPERTY(BlueprintReadOnly, Category=Components)
	UCameraComponent* CameraComp;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Components)
	USkeletalMeshComponent* Mesh1P;
	UPROPERTY()
	UPawnSensingComponent* SensingComponent;
	UPROPERTY(EditDefaultsOnly,Category=Monster)
	AActor* MonsterActor;
	
	UPROPERTY(EditDefaultsOnly, Category="Input")
	UInputMappingContext* InputMapping;
	UPROPERTY(EditDefaultsOnly, Category="Input")
	UInputAction* IA_Move;
	UPROPERTY(EditDefaultsOnly, Category="Input")
	UInputAction* IA_Look;
	UPROPERTY(EditDefaultsOnly, Category="Input")
	UInputAction* IA_ShootWeapon;
	UPROPERTY(EditDefaultsOnly, Category="Input")
	UInputAction* IA_CycleAmmoType;
		
	void Move(const FInputActionInstance& Instance);
	void Look(const FInputActionValue& Value);
	
	UFUNCTION(BlueprintCallable)
	void ShootWeapon();
	void StopShootWeapon();
	void CycleAmmoType();

	bool bCaught;
	FVector FaceDeath;
};
