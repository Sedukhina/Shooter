// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include <Characters/BaseCharacter.h>

AWeapon::AWeapon()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("WeaponRoot"));

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon Mesh"));
	WeaponMesh->SetupAttachment(RootComponent);

	CurrentAmmo = MaxAmmo;

	EquippedSocketName = CharacterRightArmItemSocket;
	UnequippedSocketName = CharacterUnequippedItemSocket;
}

void AWeapon::Shot()
{
	bIsFiring = true;
	GetWorld()->GetTimerManager().SetTimer(ShotTimer, this, &AWeapon::OnShotTimerElapsed, ShotTimerInterval, false);
	CurrentAmmo--;
}

int32 AWeapon::GetAmmo() const
{
	return CurrentAmmo;
}

int32 AWeapon::GetMaxAmmo() const
{
	return MaxAmmo;
}

void AWeapon::SetAmmo(int32 NewAmount)
{
	CurrentAmmo = NewAmount;
}

bool AWeapon::CanShoot() const
{
	return CurrentAmmo > 0 && !bIsReloading && !bIsFiring;
}

EEquipableItemType AWeapon::GetItemType() const
{
	return ItemType;
}

EAmmoType AWeapon::GetAmmoType() const
{
	return AmmoType;
}

void AWeapon::StartReload()
{
	ABaseCharacter* CharacterOwner = StaticCast<ABaseCharacter*>(GetOwner());

	bIsReloading = true;
	if (IsValid(CharacterReloadMontage))
	{
		float Duration = CharacterOwner->PlayAnimMontage(CharacterReloadMontage);
		GetWorld()->GetTimerManager().SetTimer(ReloadTimer, this, &AWeapon::EndReload, Duration, false);
	}
	else
	{
		EndReload();
	}
}

void AWeapon::EndReload()
{
	bIsReloading = false;
	if (OnReloadComplete.IsBound())
	{
		OnReloadComplete.Broadcast();
	}
}

bool AWeapon::IsReloading() const
{
	return bIsReloading;
}

bool AWeapon::IsSlotCompatible(EEquipmentSlots Slot) const
{
	return CompatableEquipmentSlots.Contains(Slot);
}

FName AWeapon::GetEquippedSocketName() const
{
	return EquippedSocketName;
}

FName AWeapon::GetUnequippedSocketName() const
{
	return UnequippedSocketName;
}

void AWeapon::OnShotTimerElapsed()
{
	bIsFiring = false;
}
