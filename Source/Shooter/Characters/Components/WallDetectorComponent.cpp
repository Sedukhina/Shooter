// Fill out your copyright notice in the Description page of Project Settings.


#include "WallDetectorComponent.h"
#include "GameFramework/Character.h"
#include <Components/CapsuleComponent.h>
#include "ShooterTypes.h"
#include <Characters/PlayerCharacter.h>

void UWallDetectorComponent::BeginPlay()
{
	Super::BeginPlay();
	CachedCharacterOwner = StaticCast<APlayerCharacter*>(GetOwner());
}

bool UWallDetectorComponent::DetectWall(OUT FWall& Wall)
{
	FCollisionQueryParams SweepsParams;
	SweepsParams.bTraceComplex = 1;
	SweepsParams.AddIgnoredActor(CachedCharacterOwner.Get());

	// Used to make a small offset of the floor to avoid unwanted collisions
	float GroundOffset = 2.f;
	UCapsuleComponent* PlayerCapsule = CachedCharacterOwner->GetCapsuleComponent();
	FVector OwnerBottom = CachedCharacterOwner->GetActorLocation() - FVector::UpVector * (PlayerCapsule->GetScaledCapsuleHalfHeight() + GroundOffset);

	// First check is to find wall bottom
	FCollisionShape ForwardSphere = FCollisionShape::MakeSphere(PlayerCapsule->GetScaledCapsuleRadius());
	FVector ForwardCheckStart = OwnerBottom + FVector::UpVector * (PlayerCapsule->GetScaledCapsuleRadius());
	FVector ForwardCheckEnd = ForwardCheckStart + CachedCharacterOwner->GetActorForwardVector() * MaxDistanceToWall;
	// Forward check to find if its any wall that fits
	FHitResult ForwardCheckHitResult;
	if (!GetWorld()->SweepSingleByChannel(ForwardCheckHitResult, ForwardCheckStart, ForwardCheckEnd, FQuat::Identity, ECC_Climbing, ForwardSphere, SweepsParams))
	{
		return false;
	}
	//DrawDebugSphere(GetWorld(), ForwardCheckHitResult.Location, PlayerCapsule->GetScaledCapsuleRadius(), 32, FColor::Red, false, 10.f);

	// Saving wall normal
	FVector WallNormal = ForwardCheckHitResult.ImpactNormal;
	// Calculating wall up 
	FVector WallUp = FVector::CrossProduct(FVector::UpVector, WallNormal);
	WallUp.Normalize();
	WallUp = -FVector::CrossProduct(WallUp, WallNormal);
	WallUp.Normalize();

	//float WallAngle 

	// Few checks along the wall to check if its actually a wall
	float Step = MinWallHeight / WallCheckTraceAmount;
	for (size_t i = 1; i < WallCheckTraceAmount; i++)
	{
		ForwardCheckStart += WallUp * Step;
		ForwardCheckEnd += WallUp * Step;
		if (!GetWorld()->SweepSingleByChannel(ForwardCheckHitResult, ForwardCheckStart, ForwardCheckEnd, FQuat::Identity, ECC_Climbing, ForwardSphere, SweepsParams))
		{
			return false;
		}
		if (ForwardCheckHitResult.ImpactNormal != WallNormal)
		{
			return false;
		}
		//DrawDebugSphere(GetWorld(), ForwardCheckHitResult.Location, PlayerCapsule->GetScaledCapsuleRadius(), 32, FColor::Green, false, 10.f);
	}

	// Overlap Check to 
	FCollisionShape OverlapCheckCapsule = FCollisionShape::MakeCapsule(PlayerCapsule->GetScaledCapsuleRadius(), PlayerCapsule->GetScaledCapsuleHalfHeight());
	FVector OverlapLocation = ForwardCheckHitResult.Location + ForwardCheckHitResult.ImpactNormal;
	if (GetWorld()->OverlapBlockingTestByProfile(OverlapLocation, ForwardCheckHitResult.Component->GetComponentQuat(), FName("Pawn"), OverlapCheckCapsule, SweepsParams))
	{
		//DrawDebugCapsule(GetWorld(), OverlapLocation, PlayerCapsule->GetScaledCapsuleHalfHeight(), PlayerCapsule->GetScaledCapsuleRadius(), ForwardCheckHitResult.Component->GetComponentQuat(), FColor::Cyan, false, 10.f);
		return false;
	}
	//DrawDebugCapsule(GetWorld(), OverlapLocation, PlayerCapsule->GetScaledCapsuleHalfHeight(), PlayerCapsule->GetScaledCapsuleRadius(), FQuat::Identity, FColor::Purple, false, 10.f);

	// Filling wall struct
	Wall.ComponentWallAttachedTo = ForwardCheckHitResult.Component;
	Wall.RelativeLocationToAttach  = OverlapLocation - Wall.ComponentWallAttachedTo->GetComponentLocation();
	Wall.Rotation = (WallNormal * FVector(-1)).ToOrientationQuat();
	Wall.Normal = WallNormal;
	Wall.WallUp = WallUp;
	return true;
}
