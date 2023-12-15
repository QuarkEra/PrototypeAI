// Fill out your copyright notice in the Description page of Project Settings.


#include "GodCharacter.h"

#include "Director.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GunsOfDinosaurs/Weapons/GodWeapon.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISense_Sight.h"
#include "Perception/PawnSensingComponent.h"

// Sets default values
AGodCharacter::AGodCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(RootComponent);
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh1P"));
	Mesh1P->SetupAttachment(CameraComp);
	SensingComponent = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("LineOfSightChecker"));
	
	if (SensingComponent)
	{
		SensingComponent->SetPeripheralVisionAngle(25);
	}
	
	LOSMultiplier = 0.1;
	DistanceToAlien = 1;
	
	SetInstigator(this);
	SetupStimulusSource();
}

void AGodCharacter::ReceiveNewDirector(ADirector* NewDirector)
{
	if (ensure(NewDirector != nullptr))
	{
		this->MyDirector = NewDirector;
	}
}


// Called when the game starts or when spawned
void AGodCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (MyDirector->GivePlayerAlien() != nullptr)
	{
		AlienCharacter = MyDirector->GivePlayerAlien();
	}
}

void AGodCharacter::SetupStimulusSource() {
	StimulusSource = CreateDefaultSubobject< UAIPerceptionStimuliSourceComponent >( TEXT( "Stimulus ") );
	if ( StimulusSource ) {
		StimulusSource->RegisterForSense( TSubclassOf< UAISense_Sight >() );
		StimulusSource->RegisterWithPerceptionSystem();
	}
}

void AGodCharacter::Move(const FInputActionInstance& Instance)
{
	if (Controller)
	{
		const FVector2D MoveValue = Instance.GetValue().Get<FVector2D>();
		//const FRotator MoveRotation = FRotator(0, GetControlRotation().Yaw, 0);
        
		if (MoveValue.X != 0.0f)
		{
			//const FVector MoveDirection = MoveRotation.RotateVector(FVector::RightVector);
			AddMovementInput(GetActorRightVector(), MoveValue.X);
		}
		if (MoveValue.Y != 0.0f)
		{
			//const FVector MoveDirection = MoveRotation.RotateVector(FVector::ForwardVector);
			AddMovementInput(GetActorForwardVector(), MoveValue.Y);
		}
	}
}

void AGodCharacter::Look(const FInputActionValue& Value)
{
	if (Controller->IsLocalController() && Controller)
	{
		FVector2D LookValue = Value.Get<FVector2D>();

		if (LookValue.X != 0.0)
		{
			if (LookValue.X < 0.05f && LookValue.X > -0.05f)
			{
				LookValue.X = 0.0;
			}
			AddControllerYawInput(LookValue.X);
		}

		if (LookValue.Y != 0.0)
		{
			if (LookValue.Y < 0.05f && LookValue.Y > -0.05f)
			{
				LookValue.Y = 0.0;
			}
			// Negating to disable inverted Y-Axis
			AddControllerPitchInput(-LookValue.Y);
		}
	}
}

void AGodCharacter::ShootWeapon()
{
	if (CurrentWeapon != nullptr)
	{
		CurrentWeapon->Shoot();
		if (CurrentWeapon->IsHasAmmo())
		{
			MakeNoise(1, this, GetActorLocation());
		}
	}
}

void AGodCharacter::StopShootWeapon()
{
	if (CurrentWeapon != nullptr)
	{
		CurrentWeapon->StopShoot();
	}
}

void AGodCharacter::CycleAmmoType()
{
	if (CurrentWeapon != nullptr)
	{
		CurrentWeapon->CycleAmmoType();
	}
}


// Called every frame
void AGodCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// @TODO: obviously take out of tick and use timer to handle smooth lerp 
	if (bCaught)
	{
		FRotator NewRot = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), FaceDeath);
		APlayerController* PC = Cast<APlayerController>(GetController());
		if (PC)
		{
			PC->SetControlRotation(
				FMath::RInterpTo(
					PC->GetControlRotation(),
					NewRot,
					UGameplayStatics::GetWorldDeltaSeconds(GetWorld()),
					10));
		}
		if (CameraComp)
		{
			CameraComp->SetFieldOfView(
				FMath::FInterpTo(
					CameraComp->FieldOfView,
					CameraComp->FieldOfView - 5,
					UGameplayStatics::GetWorldDeltaSeconds(GetWorld()),
					5)
					);
		}
	}
	// check for line of sight from player to monster
	if (ensure(SensingComponent != nullptr))
	{
		const float Distance = GetHorizontalDistanceTo(AlienCharacter);
		if (Distance < 1000)
		{
			double Multiplier = DistanceToAlien;
			Multiplier = FMath::GetMappedRangeValueClamped(FVector2d(1000, 0), FVector2d(0, Multiplier), Distance);
			Multiplier = FMath::Clamp(Multiplier, 0, DistanceToAlien);
			MyDirector->ChangeMenaceGauge(Multiplier * UGameplayStatics::GetWorldDeltaSeconds(GetWorld()));
			UE_LOG(LogTemp, Display, TEXT("Multiplier %f"), Multiplier);
		}
		if (SensingComponent->HasLineOfSightTo(AlienCharacter))
		{
			MyDirector->ChangeMenaceGauge(LOSMultiplier * UGameplayStatics::GetWorldDeltaSeconds(GetWorld()));
		}
	}
	//@TODO: Update the noise making for AI Sense Perception
	// check for speed and make noise
	if (const float Speed = UKismetMathLibrary::VSize(GetCharacterMovement()->Velocity); Speed > 300)
	{
		MakeNoise(0.015, GetInstigator(), GetActorLocation(), 1000.f);
	}
}

// Called to bind functionality to input
void AGodCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (const APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0))
	{
		if (const ULocalPlayer* LocalPlayer = PC->GetLocalPlayer())
		{
			if (UEnhancedInputLocalPlayerSubsystem* InputSystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
			{
				if (InputMapping != nullptr)
				{
					InputSystem->ClearAllMappings();
					InputSystem->AddMappingContext(InputMapping, 0);

					if (UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerInputComponent))
					{
						Input->BindAction(IA_Move, ETriggerEvent::Triggered, this, &AGodCharacter::Move);
						Input->BindAction(IA_Look, ETriggerEvent::Triggered, this, &AGodCharacter::Look);
						Input->BindAction(IA_ShootWeapon, ETriggerEvent::Started, this, &AGodCharacter::ShootWeapon);
						Input->BindAction(IA_CycleAmmoType, ETriggerEvent::Triggered, this, &AGodCharacter::CycleAmmoType);
						Input->BindAction(IA_ShootWeapon, ETriggerEvent::Completed, this, &AGodCharacter::StopShootWeapon);
					}
				}
			}
		}
	}
}

void AGodCharacter::CharacterCaught(const FVector& CatcherLocation)
{
	bCaught = true;
	FaceDeath = CatcherLocation;
	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		DisableInput(PC);
	}
	
}


