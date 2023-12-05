// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterCharacter.h"

#include "AI_MonsterController.h"
#include "FlickeringLight.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Perception/PawnSensingComponent.h"
#include "Sound/SoundCue.h"



// Sets default values
AMonsterCharacter::AMonsterCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensing"));

	LongRangeSight = 1250.f;
	LongRangeFieldOfView = 65.f;
	HearingRange = 40000;
	HearingRangeLineOfSight = 50000;

	PawnSensingComponent->bOnlySensePlayers = true;
	PawnSensingComponent->SightRadius = LongRangeSight;
	PawnSensingComponent->SetPeripheralVisionAngle(LongRangeFieldOfView);
	PawnSensingComponent->HearingThreshold = HearingRange;
	PawnSensingComponent->LOSHearingThreshold = HearingRangeLineOfSight;
	FlickerRadius = 1500;

	//PawnSensingComponent->SetSensingUpdatesEnabled(false);
	
}

// Called when the game starts or when spawned
void AMonsterCharacter::BeginPlay()
{
	Super::BeginPlay();

	// This actor handles flickering lights by distance
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AFlickeringLight::StaticClass(), FoundFlickeringLights);
	
	Scream = LoadObject<USoundCue>(nullptr, TEXT("/Game/Audio/Monster/SC_MonsterScream.SC_MonsterScream"));
	HostileScream = LoadObject<USoundCue>(nullptr, TEXT("/Game/Audio/BL-FREE22_Creatures_Designed_Cue.BL-FREE22_Creatures_Designed_Cue"));

	this->OnTakeAnyDamage.AddDynamic(this, &AMonsterCharacter::HandleDamage);
}

void AMonsterCharacter::MonsterScream() const
{
	UGameplayStatics::PlaySoundAtLocation(this, Scream, GetActorLocation(), GetActorRotation());
}

void AMonsterCharacter::MonsterHostileScream() const
{
	UGameplayStatics::SpawnSoundAttached(HostileScream, RootComponent);
}

void AMonsterCharacter::HandleDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
	AController* InstigatedBy, AActor* DamageCauser)
{
	AAI_MonsterController* MyController = Cast<AAI_MonsterController>(GetController());
	if (MyController)
	{
		MyController->StartEscaping();
	}
}

void AMonsterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (FoundFlickeringLights.Num() > 0)
	{
		for (AActor* Light : FoundFlickeringLights)
		{
			if (Light != nullptr)
			{
				FVector LightLocation = Light->GetActorLocation();
				FVector MyLocation = GetActorLocation();

				double VectorDifference = UKismetMathLibrary::Vector_Distance2D(LightLocation, MyLocation);
				
				AFlickeringLight* FlickeringLight = Cast<AFlickeringLight>(Light);
			
				if (VectorDifference <= PawnSensingComponent->SightRadius)
				{
					FlickeringLight->ReceiveDistance(VectorDifference, PawnSensingComponent->SightRadius);
					FlickeringLight->SetShouldFlicker(true);
					//FlickeringLight->ShouldChangeIntensity = true;
				}
				else
				{
					FlickeringLight->SetShouldFlicker(false);
					//FlickeringLight->ShouldChangeIntensity = false;
				}
			}
		}
	}
	//DrawDebugSphere(GetWorld(), GetActorLocation(), 32.f, 12, FColor::White, false, 0, 0, 1);
}


