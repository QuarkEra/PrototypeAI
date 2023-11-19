// Fill out your copyright notice in the Description page of Project Settings.


#include "GodCharacter.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Camera/CameraComponent.h"
#include "GunsOfDinosaurs/Weapons/GodWeapon.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AGodCharacter::AGodCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(RootComponent);
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh1P"));
	Mesh1P->SetupAttachment(CameraComp);
	SetInstigator(this);

}


// Called when the game starts or when spawned
void AGodCharacter::BeginPlay()
{
	Super::BeginPlay();
	
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
						Input->BindAction(IA_ShootWeapon, ETriggerEvent::Triggered, this, &AGodCharacter::ShootWeapon);
						Input->BindAction(IA_CycleAmmoType, ETriggerEvent::Triggered, this, &AGodCharacter::CycleAmmoType);
					}
				}
			}
		}
	}
}

