// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Weapons/PhysicalWeapon.h"
#include "PhysicalWeapon.h"
#include <Characters/BaseCharacter.h>
#include "Projectile.h"

void APhysicalWeapon::Shot()
{
	if (IsValid(ProjectileType))
	{
		ABaseCharacter* CharacterOwner = StaticCast<ABaseCharacter*>(GetOwner());
		AController* Controller = CharacterOwner->GetController<APlayerController>();
		if (!IsValid(Controller))
		{
			return;
		}

		FVector Location = WeaponMesh->GetSocketLocation(WeaponMuzzleSocket);

		FVector PlayerViewPoint;
		FRotator PlayerViewRotation;
		Controller->GetPlayerViewPoint(PlayerViewPoint, PlayerViewRotation);
		FRotator Rotation = PlayerViewRotation;
		FActorSpawnParameters SpawnInfo;
		AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileType, Location, Rotation, SpawnInfo);
		Super::Shot();
		if (IsValid(Projectile))
		{
			Projectile->SetOwner(GetOwner());
			Projectile->LaunchProjectile(Rotation.Vector());
		}
	}
}
