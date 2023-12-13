// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MonsterCharacter.generated.h"

class UAIPerceptionComponent;
class AFlickeringLight;
class AAI_MonsterController;
class UPawnSensingComponent;
class USoundCue;




UCLASS()
class GUNSOFDINOSAURS_API AMonsterCharacter : public ACharacter
{
	GENERATED_BODY()
	
public:
	// Sets default values for this character's properties
	UPROPERTY(EditDefaultsOnly, Category=AiMonster)
	bool bEnableFlickeringLights;
	
	AMonsterCharacter();
	void MonsterScream() const;
	void MonsterHostileScream() const;
	float GetSafestFlameDistance();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UFUNCTION()
	void HandleDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);
	
	UPROPERTY(EditAnywhere, Category = AiMonster)
	float						LongRangeSight;
	UPROPERTY(EditAnywhere, Category = AiMonster)
	float						LongRangeFieldOfView;
	UPROPERTY(EditAnywhere, Category = AiMonster)
	float						HearingRange;
	UPROPERTY(EditAnywhere, Category = AiMonster)
	float						HearingRangeLineOfSight;
	UPROPERTY(EditAnywhere, Category = AiMonster)
	float						SafestFlameDistance;
	UPROPERTY(EditAnywhere, Category = AiMonster)
	double						FlickerRadius;
	
	UPROPERTY(EditAnywhere)
	UPawnSensingComponent*		PawnSensingComponent;
	UPROPERTY(EditDefaultsOnly, Category="Audio")
	USoundCue*					Scream;
	UPROPERTY(EditDefaultsOnly, Category="Audio")
	USoundCue*					HostileScream;
	UPROPERTY(VisibleAnywhere)
	TArray<AActor*>				FoundFlickeringLights;

};
