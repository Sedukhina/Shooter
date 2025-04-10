// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BaseCharacterMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTER_API UBaseCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;

	virtual float GetMaxSpeed() const override;

	virtual void OnOutOfStamina();

	// Functions to get current character state
	bool IsSprinting();

	// Sprint
	bool CanSprint();
	void StartSprinting();
	void StopSprinting();

protected:
	TWeakObjectPtr<class ABaseCharacter> CachedBaseCharacterOwner;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ClampMin = 0.0f, UIMin = 0.0f))
	float SprintSpeed = 1000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ClampMin = 0.0f, UIMin = 0.0f))
	float OutOfStaminaSpeed = 200.0f;

	// Character current state
	bool bIsSprinting = false;
};
