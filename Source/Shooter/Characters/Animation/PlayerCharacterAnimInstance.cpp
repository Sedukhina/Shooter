// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Animation/PlayerCharacterAnimInstance.h"
#include "Characters/PlayerCharacter.h"
#include "Characters/Components/PlayerCharacterMovementComponent.h"

void UPlayerCharacterAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();
	CachedPlayerOwner = StaticCast<APlayerCharacter*>(TryGetPawnOwner());
}

void UPlayerCharacterAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (CachedPlayerOwner.IsValid())
	{
		UPlayerCharacterMovementComponent* OwnerCharacterMovementComponent = CachedPlayerOwner->GetPlayerCharacterMovement();
		bIsClimbing = OwnerCharacterMovementComponent->IsClimbing();
	}
}
