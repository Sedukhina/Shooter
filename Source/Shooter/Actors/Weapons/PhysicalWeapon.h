// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Weapons/Weapon.h"
#include "PhysicalWeapon.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTER_API APhysicalWeapon : public AWeapon
{
	GENERATED_BODY()
	
public:
	virtual void Shot() override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<class AProjectile> ProjectileType;
};
