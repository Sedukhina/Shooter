// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/Components/BaseCharacterMovementComponent.h"
#include "Characters/Components/WallDetectorComponent.h"
#include "PlayerCharacterMovementComponent.generated.h"


UENUM(BlueprintType)
enum class ECustomMovementMode : uint8
{
	CMOVE_None UMETA(DisplayName = "None"),
	CMOVE_Sliding UMETA(DisplayName = "Sliding"),
	CMOVE_Climbing UMETA(DisplayName = "Climbing"),
	CMOVE_Max UMETA(Hidden)
};

USTRUCT()
struct FSlideParams
{
	GENERATED_BODY()

	float SlidingDuratiuon;
	float SlidingLoopStartTime;
	FVector SlidingStartLocation;
	FVector SlidingTargetLocation;
};

/**
 * 
 */
UCLASS()
class SHOOTER_API UPlayerCharacterMovementComponent : public UBaseCharacterMovementComponent
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;
	virtual void OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode) override;

	virtual float GetMaxSpeed() const override;

	virtual void OnOutOfStamina() override;

	virtual bool CanSlide(OUT FSlideParams& SlideParams);
	virtual void StartSliding(FSlideParams Params);
	virtual void EndSliding();
	bool IsSliding() const;

	void AttachToWall(struct FWall& Wall);
	void DetachFromWall();
	bool IsClimbing() const;
	FVector GetCurrentWallUpVector() const;
	FVector GetCurrentWallLeftVector() const;

protected:
	TWeakObjectPtr<class APlayerCharacter> CachedPlayerCharacterOwner;

	virtual void PhysCustom(float DelatTime, int32 Iterations) override;
	virtual void PhysClimbing(float DelatTime, int32 Iterations);
	virtual void PhysSliding(float DelatTime, int32 Iterations);

	// Variables for sliding func
	FTimerHandle SlidingTimer;
	FSlideParams CurrentSlideParams;

	// Sliding parametrs
	UPROPERTY(EditDefaultsOnly, Category = "PlayerCharacterMovement|Slide", meta = (ClampMin = 0.f, UIMin = 0.f))
	float MaxSlidingDistance = 300.f;
	UPROPERTY(EditDefaultsOnly, Category = "PlayerCharacterMovement|Slide", meta = (ClampMin = 0.f, UIMin = 0.f))
	float MinSlidingDistance = 200.f;
	UPROPERTY(EditDefaultsOnly, Category = "PlayerCharacterMovement|Slide", meta = (ClampMin = 0.f, UIMin = 0.f))
	float MaxSlidingDistanceAnimStartTime = 0.f;
	UPROPERTY(EditDefaultsOnly, Category = "PlayerCharacterMovement|Slide", meta = (ClampMin = 0.f, UIMin = 0.f))
	float MinSlidingDistanceAnimStartTime = 0.5f;
	UPROPERTY(EditDefaultsOnly, Category = "PlayerCharacterMovement|Slide", meta = (ClampMin = 0.f, UIMin = 0.f))
	float GetUpFromSlideDistance = 130.f;
	UPROPERTY(EditDefaultsOnly, Category = "PlayerCharacterMovement|Slide")
	UCurveFloat* SlidingCurve;

	// Climbing
	FWall CurrentWall;
	UPROPERTY(EditDefaultsOnly, Category = "PlayerCharacterMovement|Climbing", meta = (ClampMin = 0.f, UIMin = 0.f))
	float ClimbingSpeed = 300.f;
	UPROPERTY(EditDefaultsOnly, Category = "PlayerCharacterMovement|Climbing", meta = (ClampMin = 0.f, UIMin = 0.f))
	float DistanceToWallUpToPullUp = 70.f;
};
