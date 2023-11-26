// Fill out your copyright notice in the Description page of Project Settings.


#include "Director.h"

#include "AI_MonsterController.h"
#include "MonsterCharacter.h"
#include "NavigationSystem.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ADirector::ADirector()
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

void ADirector::GetNewAction()
{
	GiveNewTask();
}

void ADirector::GiveNewTask()
{
	if (MenaceGauge > 66)
	{
		BackOff();
	}
	else if (MenaceGauge <= 65)
	{
		MonsterAI->LookAround();
	}
}

void ADirector::BackOff()
{
	this->MenaceGauge = 0.f;
	MonsterAI->SetNewState(EMonsterState::Idle, nullptr);
}

/*
FNavLocation ADirector::GetRandomPointsAroundPlayer(APawn* PlayerPawn)
{
	if (NavSys)
	{
		FNavLocation RandomLocation;
		float Radius = 1000.f;

		FVector Origin = UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->GetActorLocation();
		
		UE_LOG(LogTemp, Display, TEXT("Director Searching...%f"), Radius)
		
		if (NavSys->GetRandomReachablePointInRadius(Origin, Radius, RandomLocation))
		{
			// Debug for radius visual and a direct spot to head to within that radius
			/*DrawDebugSphere(
				GetWorld(),
				Origin,
				Radius,
				112,
				FColor::Green,
				false,
				4.f,
				0,
				1);
			DrawDebugSphere(
				GetWorld(),
				RandomLocation,
				32,
				12,
				FColor::Blue,
				false,
				4.f,
				0,
				1);
				
			return RandomLocation;
		}
	}
	return {};
}
*/
	
// Called every frame
void ADirector::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

