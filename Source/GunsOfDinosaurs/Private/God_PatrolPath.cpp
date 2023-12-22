// Fill out your copyright notice in the Description page of Project Settings.


#include "God_PatrolPath.h"

/*
====================
AGod_PatrolPath::AGod_PatrolPath
====================
*/
AGod_PatrolPath::AGod_PatrolPath()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

/*
====================
AGod_PatrolPath::GetPatrolPoint
====================
*/
FVector AGod_PatrolPath::GetPatrolPoint( int const index ) const {
	return PatrolPoints[ index ];
}

/*
====================
AGod_PatrolPath::Num
====================
*/
int AGod_PatrolPath::Num() const {
	return PatrolPoints.Num();
}


