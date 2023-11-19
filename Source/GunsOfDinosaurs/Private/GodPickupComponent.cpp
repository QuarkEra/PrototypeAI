// Fill out your copyright notice in the Description page of Project Settings.


#include "GodPickupComponent.h"

#include "GodCharacter.h"


// Sets default values for this component's properties
UGodPickupComponent::UGodPickupComponent()
{
	SphereRadius = 32.f;
}


// Called when the game starts
void UGodPickupComponent::BeginPlay()
{
	Super::BeginPlay();

	OnComponentBeginOverlap.AddDynamic(this, &UGodPickupComponent::OnSphereBeginOverlap);
	
}

void UGodPickupComponent::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (AGodCharacter* GodCharacter = Cast<AGodCharacter>(OtherActor); GodCharacter != nullptr)
	{
		OnPickUp.Broadcast(GodCharacter);

		OnComponentBeginOverlap.RemoveAll(this);
	}
}
