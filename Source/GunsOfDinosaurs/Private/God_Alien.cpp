// Fill out your copyright notice in the Description page of Project Settings.


#include "God_Alien.h"

// Sets default values
AGod_Alien::AGod_Alien()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AGod_Alien::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGod_Alien::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AGod_Alien::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

UBehaviorTree* AGod_Alien::GetBehaviourTree()
{
	return Tree;
}

