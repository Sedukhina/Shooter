// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Weapons/TraceWeapon.h"
#include "WeaponBarellComponent.h"
#include "Characters/BaseCharacter.h"

ATraceWeapon::ATraceWeapon()
{
	WeaponBarell = CreateDefaultSubobject<UWeaponBarellComponent>(TEXT("Weapon Barell"));
	WeaponBarell->SetupAttachment(WeaponMesh, WeaponMuzzleSocket);
}

void ATraceWeapon::Shot()
{
	ABaseCharacter* CharacterOwner = StaticCast<ABaseCharacter*>(GetOwner());
	FVector ShotLocation;
	FVector ShotRotation;

	APlayerController* Controller = CharacterOwner->GetController<APlayerController>();
	if (IsValid(Controller))
	{
		FRotator PlayerViewRotation;
		Controller->GetPlayerViewPoint(ShotLocation, PlayerViewRotation);
		ShotRotation = PlayerViewRotation.RotateVector(FVector::ForwardVector);
	}
	else
	{
		ShotLocation = WeaponBarell->GetComponentLocation();
		ShotRotation = CharacterOwner->GetBaseAimRotation().RotateVector(FVector::ForwardVector);
	}

	WeaponBarell->Shot(ShotLocation, ShotRotation, Controller);

	Super::Shot();
}
