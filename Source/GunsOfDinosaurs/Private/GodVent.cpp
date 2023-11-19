// Fill out your copyright notice in the Description page of Project Settings.


#include "GodVent.h"

#include "AI_MonsterController.h"
#include "MonsterCharacter.h"
#include "Components/BoxComponent.h"
#include "Perception/PawnSensingComponent.h"

// Sets default values
AGodVent::AGodVent()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	TriggerBox->SetupAttachment(RootComponent);
	
}

// Called when the game starts or when spawned
void AGodVent::BeginPlay()
{
	Super::BeginPlay();

	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &AGodVent::OnOverlapBegin);
	TriggerBox->OnComponentEndOverlap.AddDynamic(this,&AGodVent::OnOverlapEnd);
	
}

void AGodVent::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{	
	if (bOverlapped)
	{
		return;
	}
	if (OtherActor != nullptr)
	{
		AMonsterCharacter* Monster = Cast<AMonsterCharacter>(OtherActor);
		if (Monster)
		{
			AAI_MonsterController* MonsterController = Cast<AAI_MonsterController>(Monster->GetController());
			if (MonsterController)
			{
				if (!MonsterController->IsWantsToHunt() && MonsterController->IsWantsToVent())
				{
					MonsterController->EnterVent();
					bOverlapped = true;
				}
				if (MonsterController->IsWantsToHunt() && MonsterController->IsWantsToVent())
				{
					MonsterController->ExitVent();
					bOverlapped = false;
				}
			}	
		}
	}
}

void AGodVent::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	bOverlapped = false;
	if (OtherActor != nullptr)
	{
		AMonsterCharacter* Monster = Cast<AMonsterCharacter>(OtherActor);
		if (Monster)
		{
			AAI_MonsterController* MonsterController = Cast<AAI_MonsterController>(Monster->GetController());
			if (MonsterController)
			{
				if (MonsterController->IsWantsToHunt())
				{
					MonsterController->SetWantsToVent(true);
				}
			}	
		}
	}
}

// Called every frame
void AGodVent::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

