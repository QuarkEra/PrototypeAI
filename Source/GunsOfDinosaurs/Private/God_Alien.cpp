// Fill out your copyright notice in the Description page of Project Settings.


#include "God_Alien.h"

/*
====================
AGod_Alien::AGod_Alien
====================
*/
AGod_Alien::AGod_Alien(): Tree(nullptr), PatrolPath(nullptr) {
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ChaseSpeed = 550.0f;
	WalkSpeed = 200.0f;
	
}

/*
====================
AGod_Alien::BeginPlay
====================
*/
void AGod_Alien::BeginPlay()
{
	Super::BeginPlay();
	
}

/*
====================
AGod_Alien::Tick
====================
*/
void AGod_Alien::Tick( float DeltaTime )
{
	Super::Tick(DeltaTime);

}

/*
====================
AGod_Alien::SetupPlayerInputComponent
====================
*/
void AGod_Alien::SetupPlayerInputComponent( UInputComponent* PlayerInputComponent )
{
	Super::SetupPlayerInputComponent( PlayerInputComponent );

}

/*
====================
AGod_Alien::GetChaseSpeed
====================
*/
float AGod_Alien::GetChaseSpeed() const {
	return ChaseSpeed;
}

/*
====================
AGod_Alien::GetWalkSpeed
====================
*/
float AGod_Alien::GetWalkSpeed() const
{
	return WalkSpeed;
}

/*
====================
AGod_Alien::GetBehaviourTree
====================
*/
UBehaviorTree* AGod_Alien::GetBehaviourTree()
{
	return Tree;
}

/*
====================
AGod_Alien::GetPatrolPath
====================
*/
AGod_PatrolPath* AGod_Alien::GetPatrolPath() const {
	return PatrolPath;
}

