// Fill out your copyright notice in the Description page of Project Settings.


#include "Director.h"

#include "GodCharacter.h"
#include "God_AI_Controller.h"
#include "God_Alien.h"
#include "NavigationSystem.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ADirector::ADirector(): MenaceGauge(0), PlayerCharacter(nullptr), AlienCharacter(nullptr), AlienAI(nullptr),
                        NavSys(nullptr)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void ADirector::ChangeMenaceGauge(float DeltaMenace)
{
	MenaceGauge += DeltaMenace;
	MenaceGauge = FMath::Clamp(MenaceGauge, 0, 100);
}

// Called when the game starts or when spawned
void ADirector::BeginPlay()
{
	Super::BeginPlay();

	if ( AGod_AI_Controller * const AlienCont = Cast< AGod_AI_Controller >( AlienCharacter->GetController() ) ) {
		AlienAI = AlienCont;
	}
	if ( ensure( AlienAI ) )
	{
		AlienAI->ReceiveNewDirector( this );
	}
	PlayerCharacter = Cast< AGodCharacter >( UGameplayStatics::GetPlayerCharacter( GetWorld(), 0 ) );
	if ( ensure( PlayerCharacter ) )
	{
		PlayerCharacter->ReceiveNewDirector(this);
	}

	NavSys = UNavigationSystemV1::GetCurrent(GetWorld());
}

void ADirector::GiveNewTask()
{
	if (MenaceGauge > 99)
	{
		BackOff();
	}
	else if (MenaceGauge <= 99)
	{/*
		switch (AlienAI->GetCurrentState())
		{
		case EMonsterState::Wandering_Out_Of_Vent:
			AlienAI->LookAround();
			break;
		case EMonsterState::Wandering_In_Vent:
			AlienAI->WanderVents();
			break;
		default:
			if (AlienAI->bInVent)
			{
				AlienAI->WanderVents();
			}
			else
			{
				AlienAI->LookAround();
			}
			break;	
		}
	*/}
}

ADirector::Super* ADirector::GivePlayerAlien() const {
	if (AlienCharacter != nullptr)
	{
		return Cast<AActor>(AlienCharacter);
	}
	return nullptr;
}

void ADirector::BackOff()
{
	this->MenaceGauge = 0.f;
	// MonsterAI->SetNewState(EMonsterState::Idle, nullptr);
}
	
// Called every frame
void ADirector::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

