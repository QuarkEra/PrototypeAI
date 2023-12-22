// Fill out your copyright notice in the Description page of Project Settings.


#include "GodWeapon.h"

#include "GodCharacter.h"
#include "GodGameInstance.h"
#include "MonsterCharacter.h"
#include "Components/PointLightComponent.h"
#include "Engine/DamageEvents.h"
#include "GunsOfDinosaurs/GunsOfDinosaurs.h"
#include "Kismet/GameplayStatics.h"
#include "GunsOfDinosaurs/GunsOfDinosaurs.h"

#include "Particles/ParticleSystemComponent.h"

// Sets default values
AGodWeapon::AGodWeapon()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// when created none are active 
	bIsActiveWeapon = false;
	bIsCollected = false;
	// primary ammo is default
	CurrentAmmoType = EAmmoType::Primary;
	bHasAmmo = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Scene Comp"));
	RootComponent = SceneComponent;
	
	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Skeletal Mesh"));
	SkeletalMesh->SetupAttachment(RootComponent);
	
	MuzzleParticleEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Muzzle Particle Effect"));
	MuzzleParticleEffect->SetupAttachment(SkeletalMesh);

	PointLightComponent = CreateDefaultSubobject<UPointLightComponent>(TEXT("Light Effect"));
	PointLightComponent->SetupAttachment(SkeletalMesh);
	
}

void AGodWeapon::DoHitScan(FHitResult& Hit)
{
	FVector CameraForwardVector;
	FVector ActorLocation;
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0))
	{
		CameraForwardVector = PC->GetControlRotation().Vector();
		ActorLocation = PC->PlayerCameraManager->GetCameraLocation();
	}
	FVector TraceEndLocation = ActorLocation + (CameraForwardVector * 10000.f);
        		
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(GetOwner());
	QueryParams.bTraceComplex = true;
	QueryParams.bReturnPhysicalMaterial = true;
        		
	GetWorld()->LineTraceSingleByChannel(Hit, ActorLocation, TraceEndLocation, ECC_GameTraceChannel1, QueryParams);
}

void AGodWeapon::DoPhysicalSurfaceGetFromHitScan(FHitResult Hit, UParticleSystem*& SelectedEffect)
{
	EPhysicalSurface SurfaceType = UPhysicalMaterial::DetermineSurfaceType(Hit.PhysMaterial.Get());
	SelectedEffect = nullptr;
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
		if (CurrentAmmoType == EAmmoType::Secondary)
		{
			SelectedEffect = ImpactSecondaryAmmoEffect;
		}
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), SelectedEffect, Hit.ImpactPoint, Hit.ImpactNormal.Rotation());
	}
}

bool AGodWeapon::HandleCharacterAmmo(AGodCharacter* Character)
{
	EWeaponClass CurrentWeaponClass = Character->CurrentWeapon->WeaponClass;

	const int Ammo = GetAvailableAmmo(CurrentWeaponClass, CurrentAmmoType);
	if (Ammo > 0)
	{
		ExpendAmmo(CurrentWeaponClass, CurrentAmmoType);
		return false;
	}
	
	return true;
}

void AGodWeapon::SoundSelectionByAmmoType(const AGodCharacter* Character) const
{
	switch (CurrentAmmoType) {
	case EAmmoType::Primary:
		if (ShootSound != nullptr)
		{
			UGameplayStatics::PlaySoundAtLocation(this, ShootSound, Character->GetActorLocation());
		}
		break;
	case EAmmoType::Secondary:
		if (SecondaryAmmoShootSound != nullptr)
		{
			UGameplayStatics::PlaySoundAtLocation(this, SecondaryAmmoShootSound, Character->GetActorLocation());
		}
		break;
	default:
		break;
	}
}

bool AGodWeapon::IsHasAmmo() const
{
	return bHasAmmo;
}

void AGodWeapon::CauseDamageToValidActor(AActor* HitCharacter)
{
	if (HitCharacter != nullptr)
	{
		APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);
		if (PlayerPawn != nullptr)
		{
			//UGameplayStatics::ApplyPointDamage(HitCharacter, 1, CameraForwardVector, Hit, PlayerPawn->GetInstigatorController(), this, DamageType);
			UGameplayStatics::ApplyDamage(HitCharacter, 2, PlayerPawn->GetInstigatorController(), this, DamageType);
		}
	}
}

void AGodWeapon::Shoot()
{
	// null checks
	if (!bIsCollected)
	{
		return;
	}
	
	AGodCharacter* Character = Cast<AGodCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	if (Character == nullptr || Character->GetController() == nullptr)
	{
		return;
	}

	// if ammo is > 0 for ammo type (based on weapon held) ammo is used 
	if (HandleCharacterAmmo(Character))
	{
		bHasAmmo = false;
		return;
	}

	// LineTrace
	FHitResult Hit;
	
	
	if (bIsHitScanWeapon)
	{
		DoHitScan(Hit);
		
		// Handle Damage
		const FDamageEvent DamageEvent;
		AActor* HitCharacter = Hit.GetActor();
		if (HitCharacter)
		{
			CauseDamageToValidActor(HitCharacter);
		}
		
		// Particles are here due to checking for vulnerable headshots in hit scan
		UParticleSystem* SelectedEffect;
		DoPhysicalSurfaceGetFromHitScan(Hit, SelectedEffect);
	}
	if (bFlame)
	{
		DoHitScan(Hit);
		
		// Handle Damage
		const FDamageEvent DamageEvent;
		AActor* HitCharacter = Hit.GetActor();
		if (HitCharacter)
		{
			CauseDamageToValidActor(HitCharacter);
		}
		
		//FHitResult SweepHit;
		//GetWorld()->SweepSingleByChannel(SweepHit, ActorLocation, TraceEndLocation, GetActorQuat(), ECC_EngineTraceChannel1, SphereCollider, QueryParams);		
		
		if (MuzzleParticleEffect)
		{
			MuzzleParticleEffect->ActivateSystem(true);
		}
	}
	if (PointLightComponent)
	{
		PointLightComponent->ToggleVisibility(true);
	}
	
	
	// audio components activated
	SoundSelectionByAmmoType(Character);
}

void AGodWeapon::StopShoot()
{
	if (bFlame)
	{
		if (MuzzleParticleEffect)
		{
			// This was the old and busted way i guess
			// its what searching the web gave me
			//MuzzleParticleEffect->ActivateSystem(false);
			// new hotness i suppose
			MuzzleParticleEffect->DeactivateSystem();
		}
	}
	if (PointLightComponent)
	{
		PointLightComponent->ToggleVisibility(false);
	}
}

void AGodWeapon::CycleAmmoType()
{
	if (CurrentAmmoType == EAmmoType::Primary)
	{
		CurrentAmmoType = EAmmoType::Secondary;
	}
	else if (CurrentAmmoType == EAmmoType::Secondary)
	{
		CurrentAmmoType = EAmmoType::Primary;
	}
}

// Called when the game starts or when spawned
void AGodWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

int AGodWeapon::GetAvailableAmmo(EWeaponClass MyClass, EAmmoType AmmoType) const
{
	UGodGameInstance* GodGi = Cast<UGodGameInstance>(GetGameInstance());
	if (GodGi)
	{
		return GodGi->GetAmmo(MyClass, AmmoType);
	}

	return  0;
}

void AGodWeapon::ExpendAmmo(EWeaponClass AmmoClass, EAmmoType AmmoType)
{
	UGodGameInstance* GodGi = Cast<UGodGameInstance>(GetGameInstance());
	if (GodGi)
	{
		GodGi->UseAmmo(AmmoClass, AmmoType);
	}
}


