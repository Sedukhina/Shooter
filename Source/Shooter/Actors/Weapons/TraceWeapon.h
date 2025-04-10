// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Weapons/Weapon.h"
#include "TraceWeapon.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class SHOOTER_API ATraceWeapon : public AWeapon
{
	GENERATED_BODY()
	
public:
	ATraceWeapon();

protected:
	virtual void Shot() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UWeaponBarellComponent* WeaponBarell;
};
