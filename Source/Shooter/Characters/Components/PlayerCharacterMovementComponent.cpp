#include "PlayerCharacterMovementComponent.h"
// Fill out your copyright notice in the Description page of Project Settings.

#include "Characters/PlayerCharacter.h"
#include <Curves/CurveVector.h>
#include <Components/CapsuleComponent.h>

void UPlayerCharacterMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	CachedPlayerCharacterOwner = StaticCast<APlayerCharacter*>(GetOwner());
	bOrientRotationToMovement = false;
	RotationRate = FRotator(0.0f, 540.0f, 0.0f);

}

void UPlayerCharacterMovementComponent::OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode)
{
	Super::OnMovementModeChanged(PreviousMovementMode, PreviousCustomMode);

	if (MovementMode == MOVE_Custom)
	{
		switch (CustomMovementMode)
		{
		case((uint8)ECustomMovementMode::CMOVE_Sliding):
		{
			GetWorld()->GetTimerManager().SetTimer(SlidingTimer, this, &UPlayerCharacterMovementComponent::EndSliding, CurrentSlideParams.SlidingDuratiuon);
			break;
		}
		default:
			break;
		}
	}
}

float UPlayerCharacterMovementComponent::GetMaxSpeed() const
{
	float MaxSpeed = Super::GetMaxSpeed();
	if (IsClimbing())
	{
		MaxSpeed = ClimbingSpeed;
	}
	return MaxSpeed;
}

void UPlayerCharacterMovementComponent::OnOutOfStamina()
{
	Super::OnOutOfStamina();
	DetachFromWall();
}

bool UPlayerCharacterMovementComponent::CanSlide(OUT FSlideParams& SlideParams)
{
	if (MovementMode != MOVE_Walking)
	{
		return false;
	}

	FCollisionQueryParams SweepsParams;
	SweepsParams.bTraceComplex = 1;
	SweepsParams.AddIgnoredActor(CachedPlayerCharacterOwner.Get());

	// Used to make a small offset of the floor to avoid unwanted collisions
	float GroundOffset = 2.f;
	UCapsuleComponent* PlayerCapsule = CachedPlayerCharacterOwner->GetCapsuleComponent();
	FVector OwnerBottom = CachedPlayerCharacterOwner->GetActorLocation() - FVector::UpVector * (PlayerCapsule->GetScaledCapsuleHalfHeight() - GroundOffset);

	// Forward Check — how far we can slide from current position
	float SlidingCapsuleHalfHeight = CachedPlayerCharacterOwner->GetSlidingCapsuleHalfHeight();
	FCollisionShape ForwardCapsule = FCollisionShape::MakeCapsule(PlayerCapsule->GetScaledCapsuleRadius(), SlidingCapsuleHalfHeight);
	FVector ForwardCheckStart = OwnerBottom + FVector::UpVector * SlidingCapsuleHalfHeight;
	FVector ForwardCheckEnd = ForwardCheckStart + CachedPlayerCharacterOwner->GetActorForwardVector() * MaxSlidingDistance;

	FHitResult ForwardCheckHitResult;
	if (GetWorld()->SweepSingleByChannel(ForwardCheckHitResult, ForwardCheckStart, ForwardCheckEnd, FQuat::Identity, ECC_Pawn, ForwardCapsule, SweepsParams))
	{
		// Check if distance is to short for a slide
		if (ForwardCheckHitResult.Distance < MinSlidingDistance)
		{
			return false;
		}
		ForwardCheckEnd = ForwardCheckHitResult.Location;
	}
	
	//DrawDebugCapsule(GetWorld(), ForwardCheckEnd, SlidingCapsuleHalfHeight, PlayerCapsule->GetScaledCapsuleRadius(), FQuat::Identity, FColor::Magenta, false, 10);

	FVector GetUpStartLocation = ForwardCheckEnd + (PlayerCapsule->GetScaledCapsuleHalfHeight() + GroundOffset - SlidingCapsuleHalfHeight) * FVector::UpVector;
	FVector GetUpEndLocation = GetUpStartLocation + CachedPlayerCharacterOwner->GetActorForwardVector() * GetUpFromSlideDistance;
	FCollisionShape GetUpCheckCapsule = FCollisionShape::MakeCapsule(PlayerCapsule->GetScaledCapsuleRadius(), PlayerCapsule->GetScaledCapsuleHalfHeight());

	FHitResult GetUpCheckHitResult;
	// If player cant get up after slide — return false
	if (GetWorld()->SweepSingleByChannel(GetUpCheckHitResult, GetUpStartLocation, GetUpEndLocation, FQuat::Identity, ECC_Pawn, GetUpCheckCapsule, SweepsParams))
	{
		return false;
	}
	 //DrawDebugCapsule(GetWorld(), GetUpEndLocation, PlayerCapsule->GetScaledCapsuleHalfHeight(), PlayerCapsule->GetScaledCapsuleRadius(), FQuat::Identity, FColor::Red, false, 10);

	FVector2D SourceRange(MinSlidingDistance, MaxSlidingDistance);
	FVector2D TargetRange(MinSlidingDistanceAnimStartTime, MaxSlidingDistanceAnimStartTime);
	SlideParams.SlidingLoopStartTime = FMath::GetMappedRangeValueClamped(SourceRange, TargetRange, ForwardCheckHitResult.Distance);
	SlideParams.SlidingTargetLocation = GetUpEndLocation;
	SlideParams.SlidingStartLocation = CachedPlayerCharacterOwner->GetActorLocation();
	return true;
}

void UPlayerCharacterMovementComponent::StartSliding(FSlideParams Params)
{
	CurrentSlideParams = Params;
	float SlidingHalfHeight = CachedPlayerCharacterOwner->GetSlidingCapsuleHalfHeight();
	float WalkingCapsuleHalfHeight = CachedPlayerCharacterOwner->GetWalkingCapsuleHalfHeight();
	GEngine->AddOnScreenDebugMessage(0, 2.0f, FColor::Emerald, FString::Printf(TEXT("%f"), WalkingCapsuleHalfHeight));
	float ScaledHalfHeightAdjust = WalkingCapsuleHalfHeight - SlidingHalfHeight;
	CachedPlayerCharacterOwner->GetCapsuleComponent()->SetCapsuleHalfHeight(SlidingHalfHeight);
	UpdatedComponent->MoveComponent(FVector(0.f, 0.f, -ScaledHalfHeightAdjust), UpdatedComponent->GetComponentQuat(), true, nullptr, EMoveComponentFlags::MOVECOMP_NoFlags, ETeleportType::TeleportPhysics);
	SetMovementMode(MOVE_Custom, (uint8)ECustomMovementMode::CMOVE_Sliding);
}

void UPlayerCharacterMovementComponent::EndSliding()
{
	//APlayerCharacter* DefaultCharacter = Cast<APlayerCharacter>(APlayerCharacter::StaticClass()->GetDefaultObject(true));
	float WalkingCapsuleHalfHeight = CachedPlayerCharacterOwner->GetWalkingCapsuleHalfHeight();
	float ScaledHalfHeightAdjust = WalkingCapsuleHalfHeight - CachedPlayerCharacterOwner->GetSlidingCapsuleHalfHeight();
	UpdatedComponent->MoveComponent(FVector(0.f, 0.f, ScaledHalfHeightAdjust), UpdatedComponent->GetComponentQuat(), true, nullptr, EMoveComponentFlags::MOVECOMP_NoFlags, ETeleportType::TeleportPhysics);
	CachedPlayerCharacterOwner->GetCapsuleComponent()->SetCapsuleHalfHeight(WalkingCapsuleHalfHeight);
	GEngine->AddOnScreenDebugMessage(0, 2.0f, FColor::Emerald, FString::Printf(TEXT("%f"), WalkingCapsuleHalfHeight));
	SetMovementMode(MOVE_Walking);
}

bool UPlayerCharacterMovementComponent::IsSliding() const
{
	return UpdatedComponent && MovementMode == MOVE_Custom && CustomMovementMode == (uint8)ECustomMovementMode::CMOVE_Sliding;
}

void UPlayerCharacterMovementComponent::AttachToWall(FWall& Wall)
{
	CurrentWall = Wall;
	SetMovementMode(MOVE_Custom, (uint8)ECustomMovementMode::CMOVE_Climbing);

	FHitResult HitResult;
	FVector LocationDelta = Wall.RelativeLocationToAttach + Wall.ComponentWallAttachedTo->GetComponentLocation() - GetActorLocation();
	SafeMoveUpdatedComponent(LocationDelta, CurrentWall.Rotation, true, HitResult);
}

void UPlayerCharacterMovementComponent::DetachFromWall()
{
	FHitResult HitResult;
	SafeMoveUpdatedComponent(FVector(0), FQuat::Identity, true, HitResult);
	SetMovementMode(MOVE_Walking);
}

bool UPlayerCharacterMovementComponent::IsClimbing() const
{
	return UpdatedComponent&& MovementMode == MOVE_Custom && CustomMovementMode == (uint8)ECustomMovementMode::CMOVE_Climbing;
}

FVector UPlayerCharacterMovementComponent::GetCurrentWallUpVector() const
{
	if (IsClimbing())
	{
		return CurrentWall.WallUp;
	}
	return FVector();
}

FVector UPlayerCharacterMovementComponent::GetCurrentWallLeftVector() const
{
	if (IsClimbing())
	{
		return FVector::CrossProduct(CurrentWall.Normal, CurrentWall.WallUp);
	}
	return FVector();
}

void UPlayerCharacterMovementComponent::PhysCustom(float DeltaTime, int32 Iterations)
{
	switch (CustomMovementMode)
	{
	case((uint8)ECustomMovementMode::CMOVE_Sliding):
	{
		PhysSliding(DeltaTime, Iterations);
		break;
	}
	case((uint8)ECustomMovementMode::CMOVE_Climbing):
	{
		PhysClimbing(DeltaTime, Iterations);
		break;
	}
	default:
		break;
	}

	Super::PhysCustom(DeltaTime, Iterations);
}

void UPlayerCharacterMovementComponent::PhysSliding(float DelatTime, int32 Iterations)
{
	float TimeElapsed = (GetWorld()->GetTimerManager().GetTimerElapsed(SlidingTimer));
	float CurveValue = SlidingCurve->GetFloatValue(TimeElapsed);

	FVector CurrentLocation = FMath::Lerp(CurrentSlideParams.SlidingStartLocation, CurrentSlideParams.SlidingTargetLocation, CurveValue);

	FVector LocationDelta = CurrentLocation - GetActorLocation();
	FHitResult HitResult;
	SafeMoveUpdatedComponent(LocationDelta, CachedPlayerCharacterOwner->GetActorRotation(), false, HitResult);
}

void UPlayerCharacterMovementComponent::PhysClimbing(float DeltaTime, int32 Iterations)
{
	CalcVelocity(DeltaTime, 1.f, false, 2048.f);

	FVector LocationDelta = Velocity * DeltaTime;

	FCollisionQueryParams SweepsParams;
	SweepsParams.bTraceComplex = 1;
	SweepsParams.AddIgnoredActor(CachedPlayerCharacterOwner.Get());

	FQuat DesiredRotation = CurrentWall.Rotation; //* CurrentWall.ComponentWallAttachedTo->GetComponentQuat();

	UCapsuleComponent* PlayerCapsule = CachedPlayerCharacterOwner->GetCapsuleComponent();
	FCollisionShape CheckCapsule = FCollisionShape::MakeCapsule(PlayerCapsule->GetScaledCapsuleRadius(), PlayerCapsule->GetScaledCapsuleHalfHeight());
	FVector NewLocation = GetActorLocation() + LocationDelta;
	// If Character hanging on air check
	float OverlapCheckToWallOffset = 30.f;
	FVector OverlapLocation = NewLocation - OverlapCheckToWallOffset * CurrentWall.Normal;
	if (!GetWorld()->OverlapBlockingTestByProfile(OverlapLocation, DesiredRotation, FName("Pawn"), CheckCapsule, SweepsParams))
	{
		DetachFromWall();
		//DrawDebugCapsule(GetWorld(), OverlapLocation, PlayerCapsule->GetScaledCapsuleHalfHeight(), PlayerCapsule->GetScaledCapsuleRadius(), CurrentWall.ComponentWallAttachedTo->GetComponentQuat(), FColor::Purple, false, 10.f);
		return;
	}

	FHitResult HitResult;
	SafeMoveUpdatedComponent(LocationDelta, DesiredRotation, true, HitResult);

	if (HitResult.bBlockingHit)
	{
		DetachFromWall();
		return;
	}
}
