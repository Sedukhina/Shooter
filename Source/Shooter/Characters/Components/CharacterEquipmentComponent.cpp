#include "CharacterEquipmentComponent.h"
// Fill out your copyright notice in the Description page of Project Settings.

#include "Characters/BaseCharacter.h"
#include "Actors/Weapons/Weapon.h"

// Sets default values for this component's properties
UCharacterEquipmentComponent::UCharacterEquipmentComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UCharacterEquipmentComponent::Attack()
{
	if (IsValid(CurrentEquippedWeapon) && CurrentEquippedWeapon->CanShoot())
	{
		CurrentEquippedWeapon->Shot();
		if (CurrentEquippedWeapon->GetAmmo() == 0)
		{
			Reload();
		}
	}
}

void UCharacterEquipmentComponent::Reload()
{
	CurrentEquippedWeapon->StartReload();
}

bool UCharacterEquipmentComponent::IsAnyWeaponEquipped() const
{
	return IsValid(CurrentEquippedWeapon);
}

bool UCharacterEquipmentComponent::IsReloading() const
{
	return IsValid(CurrentEquippedWeapon) && CurrentEquippedWeapon->IsReloading();
}

int32 UCharacterEquipmentComponent::GetEquippedWeaponMaxAmmo() const
{
	if (IsValid(CurrentEquippedWeapon))
	{
		return CurrentEquippedWeapon->GetMaxAmmo();
	}
	return 0;
}

int32 UCharacterEquipmentComponent::GetEquippedWeaponCurrentAmmo() const
{
	if (IsValid(CurrentEquippedWeapon))
	{
		return CurrentEquippedWeapon->GetAmmo();
	}
	return 0;
}

int32 UCharacterEquipmentComponent::GetEquippedWeaponInventoryAmmo() const
{
	if (IsValid(CurrentEquippedWeapon))
	{
		EAmmoType AmmoType = CurrentEquippedWeapon->GetAmmoType();
		return AmmoArray[(uint32)AmmoType];
	}
	return 0;
}

bool UCharacterEquipmentComponent::AddEquipmentItemToSlot(const TSubclassOf<AWeapon> WeaponClass, int32 SlotIndex)
{
	if (!IsValid(WeaponClass))
	{
		return false;
	}

	AWeapon* DefaultWeaponObject = WeaponClass->GetDefaultObject<AWeapon>();
	if (!DefaultWeaponObject->IsSlotCompatible((EEquipmentSlots)SlotIndex))
	{
		return false;
	}

	if (!IsValid(ItemsArray[SlotIndex]))
	{
		AWeapon* Weapon = GetWorld()->SpawnActor<AWeapon>(WeaponClass);
		Weapon->AttachToComponent(CachedOwner->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, Weapon->GetUnequippedSocketName());
		Weapon->SetOwner(CachedOwner.Get());
		Weapon->OnReloadComplete.AddUFunction(this, FName("OnWeaponReloadComplete"));
		//Weapon->Unequip();
		ItemsArray[SlotIndex] = Weapon;
	}
	else
	{
		int32 AmmoSlotIndex = (int32)DefaultWeaponObject->GetAmmoType();
		AmmoArray[SlotIndex] += DefaultWeaponObject->GetMaxAmmo();
	}

	return true;
}

void UCharacterEquipmentComponent::EquipItemInSlot(EEquipmentSlots Slot)
{
	if (IsValid(CurrentEquippedWeapon))
	{
		CurrentEquippedWeapon->AttachToComponent(CachedOwner->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, CurrentEquippedWeapon->GetUnequippedSocketName());
	}

	CurrentEquippedWeapon = ItemsArray[(uint32)Slot];

	if (IsValid(CurrentEquippedWeapon))
	{
		CurrentEquippedWeapon->AttachToComponent(CachedOwner->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, CurrentEquippedWeapon->GetEquippedSocketName());
		CurrentSlot = Slot;
	}
	else
	{
		CurrentSlot = EEquipmentSlots::None;
	}
}

void UCharacterEquipmentComponent::EquipNextItem()
{
	uint32 CurrentSlotIndex = (uint32)CurrentSlot;
	uint32 NextSlotIndex = GetNextItemSlotIndex(CurrentSlotIndex);
	while (CurrentSlotIndex != NextSlotIndex && !IsValid(ItemsArray[NextSlotIndex]))
	{
		NextSlotIndex = GetNextItemSlotIndex(NextSlotIndex);
	}
	if (CurrentSlotIndex != NextSlotIndex)
	{
		EquipItemInSlot((EEquipmentSlots)NextSlotIndex);
	}
}

void UCharacterEquipmentComponent::EquipPreviousItem()
{
	uint32 CurrentSlotIndex = (uint32)CurrentSlot;
	uint32 PreviousSlotIndex = GetPreviousItemSlotIndex(CurrentSlotIndex);
	while (CurrentSlotIndex != PreviousSlotIndex && !IsValid(ItemsArray[PreviousSlotIndex]))
	{
		PreviousSlotIndex = GetPreviousItemSlotIndex(PreviousSlotIndex);
	}
	if (CurrentSlotIndex != PreviousSlotIndex)
	{
		EquipItemInSlot((EEquipmentSlots)PreviousSlotIndex);
	}
}

uint32 UCharacterEquipmentComponent::GetNextItemSlotIndex(uint32 CurrentSlotIndex)
{
	if (CurrentSlotIndex == ItemsArray.Num() - 1)
	{
		return 0;
	}
	else
	{
		return CurrentSlotIndex + 1;
	}
}

uint32 UCharacterEquipmentComponent::GetPreviousItemSlotIndex(uint32 CurrentSlotIndex)
{
	if (CurrentSlotIndex == 0)
	{
		return ItemsArray.Num() - 1;
	}
	else
	{
		return CurrentSlotIndex - 1;
	}
}

void UCharacterEquipmentComponent::BeginPlay()
{
	Super::BeginPlay();
	CachedOwner = StaticCast<ABaseCharacter*>(GetOwner());

	AmmoArray.AddZeroed((uint32)EAmmoType::MAX);
	for (const TPair<EAmmoType, int32>& AmmoPair : MaxAmmo)
	{
		AmmoArray[(uint32)AmmoPair.Key] = AmmoPair.Value;
	}

	CreateLoadout();
}

void UCharacterEquipmentComponent::CreateLoadout()
{
	ItemsArray.AddZeroed((uint32)EEquipmentSlots::MAX);
	for (const TPair<EEquipmentSlots, TSubclassOf<AWeapon>>& ItemPair : ItemsLoadout)
	{
		if (!IsValid(ItemPair.Value))
		{
			continue;
		}
		AddEquipmentItemToSlot(ItemPair.Value, (int32)ItemPair.Key);
	}
	EquipItemInSlot(EEquipmentSlots::PrimaryWeapon);
}

void UCharacterEquipmentComponent::OnWeaponReloadComplete()
{
	if (IsValid(CurrentEquippedWeapon))
	{
		int32 AmmoNeeded = GetEquippedWeaponMaxAmmo() - GetEquippedWeaponCurrentAmmo();
		AmmoNeeded = FMath::Min(AmmoArray[(uint32)CurrentEquippedWeapon->GetAmmoType()], AmmoNeeded);
		AmmoArray[(uint32)CurrentEquippedWeapon->GetAmmoType()] -= AmmoNeeded;
		CurrentEquippedWeapon->SetAmmo(CurrentEquippedWeapon->GetAmmo() + AmmoNeeded);
	}
}

