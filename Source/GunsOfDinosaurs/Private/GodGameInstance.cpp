// Fill out your copyright notice in the Description page of Project Settings.


#include "GodGameInstance.h"

#include "GunsOfDinosaurs/Weapons/GodWeapon.h"

UGodGameInstance::UGodGameInstance()
{
	PistolAmmo = 8;
	ShotgunPrimaryAmmo = 2;
	ShotgunSecondaryAmmo = 0;
}

int UGodGameInstance::GetAmmo(EWeaponClass WeaponClass, EAmmoType AmmoType) const
{
	switch (WeaponClass)
	{
	case EWeaponClass::Pistol:
		return PistolAmmo;
	case EWeaponClass::Shotgun:
		switch (AmmoType)
		{
		case EAmmoType::Primary:
			return ShotgunPrimaryAmmo;
			break;
		case EAmmoType::Secondary:
			return ShotgunSecondaryAmmo;
			break;
		default:
			break;
		}
		
	default:
		return 0;
	}
}

void UGodGameInstance::AddAmmo(EWeaponClass WeaponClass, int AmmoCount, EAmmoType AmmoType)
{
	switch (WeaponClass)
	{
	case EWeaponClass::Melee:
		break;
	case EWeaponClass::Pistol:
		PistolAmmo += AmmoCount;
		break;
	case EWeaponClass::Shotgun:
		switch (AmmoType)
		{
			case EAmmoType::Primary:
				ShotgunPrimaryAmmo += AmmoCount;
				break;
			case EAmmoType::Secondary:
				ShotgunSecondaryAmmo += AmmoCount;
				break;
			default:
				break;
		}
		break;
	case EWeaponClass::Bow:
		break;
	default:
		break;
	}	
}

void UGodGameInstance::UseAmmo(EWeaponClass WeaponClass, EAmmoType AmmoType)
{
	switch (WeaponClass)
	{
	case EWeaponClass::Pistol:
		PistolAmmo--;
		break;
	case EWeaponClass::Shotgun:
		if (AmmoType == EAmmoType::Primary)
		{
			ShotgunPrimaryAmmo--;
		}
		else if (AmmoType == EAmmoType::Secondary)
		{
			ShotgunSecondaryAmmo--;
		}
	default:
		break;
	}
}
