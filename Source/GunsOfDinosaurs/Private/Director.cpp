// Fill out your copyright notice in the Description page of Project Settings.


#include "Director.h"

#include "AI_MonsterController.h"
#include "GodCharacter.h"
#include "MonsterCharacter.h"
#include "NavigationSystem.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ADirector::ADirector(): PlayerCharacter(nullptr), MonsterCharacter(nullptr), MonsterAI(nullptr), NavSys(nullptr),
                        MenaceGauge(0)
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

	MonsterAI = Cast<AAI_MonsterController>(MonsterCharacter->GetController());
	if (ensure(MonsterAI))
	{
		MonsterAI->ReceiveNewDirector(this);
	}
	PlayerCharacter = Cast<AGodCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (ensure(PlayerCharacter))
	{
		PlayerCharacter->ReceiveNewDirector(this);
	}

	NavSys = UNavigationSystemV1::GetCurrent(GetWorld());
}

float ADirector::CheckMenaceGauge() const
{
	return MenaceGauge;
}

void ADirector::GiveNewTask()
{
	if (MenaceGauge > 99)
	{
		BackOff();
	}
	else if (MenaceGauge <= 99)
	{
		switch (MonsterAI->GetCurrentState())
		{
		case EMonsterState::Wandering_Out_Of_Vent:
			MonsterAI->LookAround();
			break;
		case EMonsterState::Wandering_In_Vent:
			MonsterAI->WanderVents();
			break;
		default:
			if (MonsterAI->bInVent)
			{
				MonsterAI->WanderVents();
			}
			else
			{
				MonsterAI->LookAround();
			}
			break;
		}
	}
}

ADirector::Super* ADirector::GiveMonsterActor()
{
	if (MonsterCharacter != nullptr)
	{
		return Cast<AActor>(MonsterCharacter);
	}
	return nullptr;
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

