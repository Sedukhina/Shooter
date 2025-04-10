// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "WeaponBarellComponent.generated.h"

USTRUCT(BlueprintType)
struct FDecalInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Decals")
	UMaterialInterface* DecalMaterial;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Decals")
	FVector DecalSize = FVector(15.f);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Decals")
	float DecalLifeTime = 10.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Decals")
	float DecalFadeoutTime = 5.f;
};


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SHOOTER_API UWeaponBarellComponent : public USceneComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UWeaponBarellComponent();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UCurveFloat* DamageCurve;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float FiringRange = 100.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UNiagaraSystem* BulletTraceFX;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FDecalInfo DefaultDecalInfo;

public:
	//void Shot(FVector ShotStart, FVector ShotDirection, AController* Controller);
	void Shot(FVector ShotStart, FVector ShotDirection, AController * Controller);

};
