// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponBarellComponent.h"
#include "Engine/DamageEvents.h"
#include "ShooterTypes.h"

#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Components/DecalComponent.h"
#include <Kismet/GameplayStatics.h>


UWeaponBarellComponent::UWeaponBarellComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UWeaponBarellComponent::Shot(FVector ShotStart, FVector ShotDirection, AController* Controller)
{
	//FVector ShotStart = GetComponentLocation();
	FVector ShotEnd = ShotStart + FiringRange * ShotDirection;
	FHitResult ShotResult;

	if (GetWorld()->LineTraceSingleByChannel(ShotResult, ShotStart, ShotEnd, ECC_Bullet))
	{
		//DrawDebugLine(GetWorld(), ShotStart, ShotResult.ImpactPoint, FColor(255, 0, 0), false, -1, 0, 3);
		ShotResult.GetActor()->TakeDamage(DamageCurve->GetFloatValue(ShotResult.Distance), FDamageEvent(), Controller, GetOwner());
		ShotEnd = ShotResult.ImpactPoint;

		UDecalComponent* DecalComponent = UGameplayStatics::SpawnDecalAtLocation(GetWorld(), DefaultDecalInfo.DecalMaterial, DefaultDecalInfo.DecalSize, ShotResult.ImpactPoint, ShotResult.ImpactNormal.ToOrientationRotator());
		if (IsValid(DecalComponent))
		{
			DecalComponent->SetFadeScreenSize(0.0001f);
			DecalComponent->SetFadeOut(DefaultDecalInfo.DecalLifeTime, DefaultDecalInfo.DecalFadeoutTime);
		}
	}
	else
	{
		//DrawDebugLine(GetWorld(), ShotStart, ShotEnd, FColor(0, 255, 0), false, -1, 0, 3);
	}

	FVector MuzzleLocation = GetComponentLocation();
	UNiagaraComponent* BulletTraceFXComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), BulletTraceFX, MuzzleLocation, GetComponentRotation());
	BulletTraceFXComponent->SetVectorParameter(FXParamBulletTraceEnd, ShotEnd);
}
