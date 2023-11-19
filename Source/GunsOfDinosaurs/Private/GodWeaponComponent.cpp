// Fill out your copyright notice in the Description page of Project Settings.


#include "GodWeaponComponent.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GodProjectile.h"
#include "DrawDebugHelpers.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "GunsOfDinosaurs/GunsOfDinosaurs.h"
#include "Kismet/GameplayStatics.h"


UGodWeaponComponent::UGodWeaponComponent()
{
}

void UGodWeaponComponent::AttachWeapon(AGodCharacter* TargetCharacter)
{
	Character = TargetCharacter;

	if (Character == nullptr)
	{
		return;
	}

	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepRelative, true);
	
	AttachToComponent(Character->GetMesh1P(), AttachmentRules, FName(TEXT("GripPoint")));

	if (APlayerController* PlayerController = Cast<APlayerController>(Character->GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(ShootMappingContext, 1);
		}

		if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent))
		{
			EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Triggered, this, &UGodWeaponComponent::Shoot);
		}
	}
}

void UGodWeaponComponent::Shoot()
{
	if (Character == nullptr || Character->GetController() == nullptr)
	{
		return;
	}

	if (ProjectileClass != nullptr)
	{
		UWorld* const World = GetWorld();
		if (World != nullptr)
		{
			const APlayerController* PlayerController = Cast<APlayerController>(Character->GetController());
			const FRotator SpawnRotation = PlayerController->PlayerCameraManager->GetCameraRotation();
			const FVector SpawnLocation = GetSocketLocation(FName(TEXT("GripPoint")));

			FActorSpawnParameters ActorSpawnParameters;
			ActorSpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

			World->SpawnActor<AGodProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParameters);
		}
	}

	if (bIsHitScanWeapon)
	{
		FHitResult Hit;
		FVector CameraForwardVector;
		FVector ActorLocation;
		if (APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0))
		{
			CameraForwardVector = PC->GetControlRotation().Vector();
			ActorLocation = PC->PlayerCameraManager->GetCameraLocation();
		}
		FVector TraceEndLocation = ActorLocation + (CameraForwardVector * 10000.f);
		
		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredComponent(this);
		QueryParams.AddIgnoredActor(GetOwner());
		QueryParams.bTraceComplex = true;
		QueryParams.bReturnPhysicalMaterial = true;
		
		GetWorld()->LineTraceSingleByChannel(Hit, ActorLocation, TraceEndLocation, ECC_GameTraceChannel1, QueryParams);
				
		EPhysicalSurface SurfaceType = UPhysicalMaterial::DetermineSurfaceType(Hit.PhysMaterial.Get());
		UParticleSystem* SelectedEffect = nullptr;
		switch (SurfaceType)
		{
		case SURFACE_FLESHDEFAULT:
		case SURFACE_FLESHVULNERABLE:
			SelectedEffect = ImpactFleshEffect;
			break;
		default:
			SelectedEffect = ImpactDefaultEffect;
			break;
		}

		if (SelectedEffect)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), SelectedEffect, Hit.ImpactPoint, Hit.ImpactNormal.Rotation());
		}
	}

	if (ShootSound != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ShootSound, Character->GetActorLocation());
	}

	if (ShootAnimation != nullptr)
	{
		PlayAnimation(ShootAnimation, false);
	}
}

void UGodWeaponComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (Character == nullptr)
	{
		return;
	}

	if (APlayerController* PlayerController = Cast<APlayerController>(Character->GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->RemoveMappingContext(ShootMappingContext);
		}
	}
}
