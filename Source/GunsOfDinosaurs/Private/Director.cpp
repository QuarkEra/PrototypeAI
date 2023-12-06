// Fill out your copyright notice in the Description page of Project Settings.


#include "Director.h"

#include "AI_MonsterController.h"
#include "MonsterCharacter.h"
#include "NavigationSystem.h"


// Sets default values
ADirector::ADirector(): MonsterCharacter(nullptr), MonsterAI(nullptr), NavSys(nullptr), MenaceGauge(0)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void ADirector::ChangeMenaceGauge(float DeltaMenace)
{
	MenaceGauge += DeltaMenace;
	FMath::Clamp(MenaceGauge, 0, 100);
}

// Called when the game starts or when spawned
void ADirector::BeginPlay()
{
	Super::BeginPlay();

	MonsterAI = Cast<AAI_MonsterController>(MonsterCharacter->GetController());
	if (ensure(MonsterAI))
	{
		MonsterAI->ReceiveNewDirector(this);
	}

	NavSys = UNavigationSystemV1::GetCurrent(GetWorld());
	
}

float ADirector::CheckMenaceGauge() const
{
	return MenaceGauge;
}

void ADirector::GiveNewTask()
{
	if (MenaceGauge > 66)
	{
		BackOff();
	}
	else if (MenaceGauge <= 65)
	{
		MonsterAI->FillNavPoints(TempLocation);
		MonsterAI->LookAround();
	}
}

void ADirector::BackOff()
{
	this->MenaceGauge = 0.f;
	MonsterAI->SetNewState(EMonsterState::Idle, nullptr);
}
	
// Called every frame
void ADirector::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

