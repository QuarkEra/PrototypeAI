// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MonsterCharacter.generated.h"

class AFlickeringLight;
class AAI_MonsterController;
class UPawnSensingComponent;
class USoundCue;

UCLASS()
class GUNSOFDINOSAURS_API AMonsterCharacter : public ACharacter
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditAnywhere, Category = MonsterAI)
	float LongRangeSight;
	UPROPERTY(EditAnywhere, Category = MonsterAI)
	float LongRangeFieldOfView;
	UPROPERTY(EditAnywhere, Category = MonsterAI)
	float HearingRange;
	UPROPERTY(EditAnywhere, Category = MonsterAI)
	float HearingRangeLineOfSight;
	
public:
	// Sets default values for this character's properties
	AMonsterCharacter();
	void MonsterScream() const;
	void MonsterHostileScream() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditAnywhere)
	UPawnSensingComponent* PawnSensingComponent;
	UPROPERTY(EditAnywhere, Category = MonsterAI)
	double FlickerRadius;

	UPROPERTY(EditDefaultsOnly, Category="Audio")
	USoundCue* Scream;
	UPROPERTY(EditDefaultsOnly, Category="Audio")
	USoundCue* HostileScream;
	
	UPROPERTY(VisibleAnywhere)
	TArray<AActor*> FoundFlickeringLights;

	// Any Damage is good enough for now
	UFUNCTION()
	void HandleDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);
	//UFUNCTION()
	//void HandlePointDamage(AActor* DamagedActor, float Damage, class AController* InstigatedBy, FVector HitLocation, class UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection, const class UDamageType* DamageType, AActor* DamageCauser);
 
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
