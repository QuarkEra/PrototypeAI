// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GodProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;

UCLASS()
class GUNSOFDINOSAURS_API AGodProjectile : public AActor
{
private:
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGodProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category="Components")
	USphereComponent* SphereComponent;
	UPROPERTY(EditDefaultsOnly, Category="Components")
	UProjectileMovementComponent* ProjectileMovementComponent;
	UPROPERTY(EditDefaultsOnly, Category="Components")
	UStaticMeshComponent* StaticMesh;
	
	
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
