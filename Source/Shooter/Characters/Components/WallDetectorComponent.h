// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WallDetectorComponent.generated.h"

USTRUCT()
struct FWall
{
	GENERATED_BODY()

	TWeakObjectPtr<UPrimitiveComponent> ComponentWallAttachedTo;

	FQuat Rotation;
	FVector RelativeLocationToAttach;
	FVector Normal;
	FVector WallUp;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SHOOTER_API UWallDetectorComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	TWeakObjectPtr<class ACharacter> CachedCharacterOwner;

	// Character will snap to it
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MinWallHeight = 160.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int WallCheckTraceAmount = 3;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MaxDistanceToWall = 150.f;

public:
	bool DetectWall(OUT FWall& Wall);
};
