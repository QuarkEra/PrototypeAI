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
#include "Perception/AISense_Hearing.h"
#include "Perception/AISense_Sight.h"
#include "Perception/PawnSensingComponent.h"
#include "Sound/SoundCue.h"

/*
====================
AGodCharacter::AGodCharacter
====================
*/
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

	CrouchSpeed		= 175;
	WalkSpeed		= 300;
	SprintSpeed		= 600;
	
	LOSMultiplier	= 0.1;
	DistanceToAlien = 1;
	
	SetInstigator(this);
	SetupStimulusSource();
}

/*
====================
AGodCharacter::ReceiveNewDirector
====================
*/
void AGodCharacter::ReceiveNewDirector(ADirector* NewDirector)
{
	if (ensure(NewDirector != nullptr))
	{
		this->MyDirector = NewDirector;
	}
}

/*
====================
AGodCharacter::BeginPlay
====================
*/
void AGodCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (MyDirector->GivePlayerAlien() != nullptr)
	{
		AlienCharacter = MyDirector->GivePlayerAlien();
	}

	GetCharacterMovement()->MaxWalkSpeedCrouched = CrouchSpeed;
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

/*
====================
AGodCharacter::SetupStimulusSource
====================
*/
void AGodCharacter::SetupStimulusSource() {
	StimulusSource = CreateDefaultSubobject< UAIPerceptionStimuliSourceComponent >( TEXT( "Stimulus ") );
	if ( StimulusSource ) {
		StimulusSource->RegisterForSense( TSubclassOf< UAISense_Sight >() );
		StimulusSource->RegisterWithPerceptionSystem();
	}
}

/*
====================
AGodCharacter::Move
====================
*/
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

/*
====================
AGodCharacter::Look
====================
*/
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

/*
====================
AGodCharacter::StartSprint
====================
*/
void AGodCharacter::StartSprint() {
	GetCharacterMovement()->MaxWalkSpeed = SprintSpeed; 
}

/*
====================
AGodCharacter::StopSprint
====================
*/
void AGodCharacter::StopSprint() {
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed; 
}

/*
====================
AGodCharacter::ShootWeapon
====================
*/
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

/*
====================
AGodCharacter::StopShootWeapon
====================
*/
void AGodCharacter::StopShootWeapon()
{
	if (CurrentWeapon != nullptr)
	{
		CurrentWeapon->StopShoot();
	}
}

/*
====================
AGodCharacter::CycleAmmoType
====================
*/
void AGodCharacter::CycleAmmoType()
{
	if (CurrentWeapon != nullptr)
	{
		CurrentWeapon->CycleAmmoType();
	}
}

/*
====================
AGodCharacter::LineOfSightToEnemy
====================
*/
void AGodCharacter::LineOfSightToEnemy() {
	const float Distance = GetHorizontalDistanceTo(AlienCharacter);
	if (Distance < 1000)
	{
		double Multiplier = DistanceToAlien;
		Multiplier = FMath::GetMappedRangeValueClamped(FVector2d(1000, 0), FVector2d(0, Multiplier), Distance);
		Multiplier = FMath::Clamp(Multiplier, 0, DistanceToAlien);
		MyDirector->ChangeMenaceGauge(Multiplier * UGameplayStatics::GetWorldDeltaSeconds(GetWorld()));
	}
	if (SensingComponent->HasLineOfSightTo(AlienCharacter))
	{
		MyDirector->ChangeMenaceGauge(LOSMultiplier * UGameplayStatics::GetWorldDeltaSeconds(GetWorld()));
	}
}

/*
====================
AGodCharacter::Tick
====================
*/
void AGodCharacter::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
	
	// check for line of sight from player to monster
	if ( ensure( SensingComponent != nullptr ) ){
		LineOfSightToEnemy();
	}
	if ( const float Speed = UKismetMathLibrary::VSize( GetCharacterMovement()->Velocity); Speed > WalkSpeed ) {
		UAISense_Hearing::ReportNoiseEvent( GetWorld(), GetActorLocation(), 1, GetInstigator(), 0, FName( "Default__AISense_Hearing" ) );
	}
	if ( bCaught ) {
		DeathAnimation( FaceDeath, DeltaTime );
	}
}

/*
====================
AGodCharacter::SetupPlayerInputComponent
====================
*/
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
						Input->BindAction(IA_Sprint, ETriggerEvent::Started, this, &AGodCharacter::StartSprint);
						Input->BindAction(IA_Sprint, ETriggerEvent::Completed, this, &AGodCharacter::StopSprint);
						Input->BindAction(IA_ShootWeapon, ETriggerEvent::Started, this, &AGodCharacter::ShootWeapon);
						Input->BindAction(IA_CycleAmmoType, ETriggerEvent::Triggered, this, &AGodCharacter::CycleAmmoType);
						Input->BindAction(IA_ShootWeapon, ETriggerEvent::Completed, this, &AGodCharacter::StopShootWeapon);
					}
				}
			}
		}
	}
}

/*
====================
AGodCharacter::KillPlayer
====================
*/
void AGodCharacter::KillPlayer( const FVector& CatcherLocation ) {
	bCaught = true;
	UGameplayStatics::SpawnSound2D( this, DeathSounds, 1 );
	if ( APlayerController* PC = Cast< APlayerController >( GetController() ) ) {
		DisableInput( PC );
		FaceDeath = CatcherLocation;
	}
}

/*
====================
AGodCharacter::DeathAnimation
====================
*/
void AGodCharacter::DeathAnimation(const FVector& CatcherLocation, float DeltaTime) {
	const FVector y = GetActorLocation();
	const FVector X = CatcherLocation - y;
	const FRotator deathDirection = FRotationMatrix::MakeFromX(X).Rotator();
	
	APlayerController* PC = Cast<APlayerController>( GetController() );
	if ( PC ) {
		PC->SetControlRotation( FMath::RInterpTo( PC->GetControlRotation(), deathDirection, DeltaTime, 10 ) );
	}
	if ( CameraComp ) {
		CameraComp->SetFieldOfView( FMath::FInterpTo( CameraComp->FieldOfView, DesiredFov, DeltaTime, 5 ) );
	}
}
