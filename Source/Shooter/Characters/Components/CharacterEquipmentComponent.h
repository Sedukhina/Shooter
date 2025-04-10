// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ShooterTypes.h"
#include "CharacterEquipmentComponent.generated.h"

typedef TArray<class AWeapon*, TInlineAllocator<(int32)EEquipmentSlots::MAX>> TItemsArray;
typedef TArray<int32, TInlineAllocator<(int32)EAmmoType::MAX>> TAmmoArray;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SHOOTER_API UCharacterEquipmentComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCharacterEquipmentComponent();

	void Attack();
	void Reload();

	bool IsAnyWeaponEquipped() const;
	bool IsReloading() const;

	int32 GetEquippedWeaponMaxAmmo() const;
	int32 GetEquippedWeaponCurrentAmmo() const;
	int32 GetEquippedWeaponInventoryAmmo() const;

	bool AddEquipmentItemToSlot(const TSubclassOf<AWeapon> EquipableItemClass, int32 SlotIndex);
	// Equips item in slot as CurrentWeapon
	void EquipItemInSlot(EEquipmentSlots Slot);
	void EquipNextItem();
	void EquipPreviousItem();

	uint32 GetNextItemSlotIndex(uint32 Slot);
	uint32 GetPreviousItemSlotIndex(uint32 Slot);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
	class AWeapon* CurrentEquippedWeapon;
	EEquipmentSlots CurrentSlot = EEquipmentSlots::None;

	void CreateLoadout();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TMap<EEquipmentSlots, TSubclassOf<class AWeapon>> ItemsLoadout;
	TItemsArray ItemsArray;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TMap<EAmmoType, int32> MaxAmmo;
	TAmmoArray AmmoArray;

	UFUNCTION()
	void OnWeaponReloadComplete();

	TWeakObjectPtr<class ABaseCharacter> CachedOwner;
};
