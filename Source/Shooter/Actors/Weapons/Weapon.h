// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ShooterTypes.h"
#include "Weapon.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnReloadComplete);

UCLASS(Abstract)
class SHOOTER_API AWeapon : public AActor
{
	GENERATED_BODY()

public:
	AWeapon();
	virtual void Shot();

	int32 GetAmmo() const;
	int32 GetMaxAmmo() const;
	void SetAmmo(int32 NewAmount);
	bool CanShoot() const;

	EEquipableItemType GetItemType() const;
	EAmmoType GetAmmoType() const;

	void StartReload();
	void EndReload();
	bool IsReloading() const;
	FOnReloadComplete OnReloadComplete;

	bool IsSlotCompatible(EEquipmentSlots Slot) const;

	FName GetEquippedSocketName() const;
	FName GetUnequippedSocketName() const;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EEquipableItemType ItemType = EEquipableItemType::None;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EAmmoType AmmoType = EAmmoType::None;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<EEquipmentSlots> CompatableEquipmentSlots;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USkeletalMeshComponent* WeaponMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FName UnequippedSocketName = NAME_None;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FName EquippedSocketName = NAME_None;

	UPROPERTY(EditDefaultsOnly)
	class UAnimMontage* CharacterReloadMontage;

	bool bIsReloading = false;
	FTimerHandle ReloadTimer;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = 1, UIMin = 1))
	int32 MaxAmmo = 10;

	int32 CurrentAmmo;

	bool bIsFiring = false;
	void OnShotTimerElapsed();
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float ShotTimerInterval = 1.5f;
	FTimerHandle ShotTimer;
};
