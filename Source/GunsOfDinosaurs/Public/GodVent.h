// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GodVent.generated.h"

class AMonsterCharacter;
class UPawnSensingComponent;
class UBoxComponent;

UCLASS()
class GUNSOFDINOSAURS_API AGodVent : public AActor
{
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	AGodVent();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	bool bOverlapped;
	
	UPROPERTY()
	UPawnSensingComponent* SensingComponent;
	UPROPERTY()
	APawn* ControlledPawn;
	UPROPERTY()
	AMonsterCharacter* MonsterCharacter;

	UPROPERTY(EditAnywhere)
	UBoxComponent* TriggerBox;

	UFUNCTION(BlueprintCallable)
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION(BlueprintCallable)
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
