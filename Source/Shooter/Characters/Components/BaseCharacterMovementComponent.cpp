// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Components/BaseCharacterMovementComponent.h"
#include "Characters/Components/StatsComponent.h"
#include <Characters/BaseCharacter.h>

void UBaseCharacterMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	bOrientRotationToMovement = true;
	PrimaryComponentTick.bCanEverTick = true;

	CachedBaseCharacterOwner = StaticCast<ABaseCharacter*>(GetOwner());
	CachedBaseCharacterOwner->GetStatsComponent()->OnOutOfStamina.AddUObject(this, &UBaseCharacterMovementComponent::OnOutOfStamina);
}

float UBaseCharacterMovementComponent::GetMaxSpeed() const
{
	float MaxSpeed = Super::GetMaxSpeed();
	if (bIsSprinting)
	{
		MaxSpeed = SprintSpeed;
	}
	else if (CachedBaseCharacterOwner->GetStatsComponent()->IsOutOfStamina())
	{
		MaxSpeed = OutOfStaminaSpeed;
	}
	return MaxSpeed;
}

void UBaseCharacterMovementComponent::OnOutOfStamina()
{
	StopSprinting();
}

bool UBaseCharacterMovementComponent::IsSprinting()
{
	return bIsSprinting;
}

bool UBaseCharacterMovementComponent::CanSprint()
{
	return !CachedBaseCharacterOwner->GetStatsComponent()->IsOutOfStamina();
}

void UBaseCharacterMovementComponent::StartSprinting()
{
	if (CanSprint())
	{
		bIsSprinting = true;
	}
}

void UBaseCharacterMovementComponent::StopSprinting()
{
	bIsSprinting = false;
}
