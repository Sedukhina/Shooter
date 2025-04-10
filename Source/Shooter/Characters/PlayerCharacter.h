// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/BaseCharacter.h"
#include "GenericTeamAgentInterface.h"
#include "InputAction.h"
#include "PlayerCharacter.generated.h"


DECLARE_DELEGATE_OneParam(FOnObjectOnLineOfSightFound, UInputAction*)
/**
 * 
 */
UCLASS(Blueprintable)
class SHOOTER_API APlayerCharacter : public ABaseCharacter, public IGenericTeamAgentInterface
{
	GENERATED_BODY()
	
protected:

	// Player camera
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerCharacter|Camera")
	class UCameraComponent* FollowCamera;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerCharacter|Camera")
	class USpringArmComponent* CameraBoom;

	// Custom Movement Component
	class UPlayerCharacterMovementComponent* PlayerCharacterMovementComponent;

	virtual bool CanJumpInternal_Implementation() const override;

	UPROPERTY(EditDefaultsOnly, Category = "PlayerCharacterMovement|Slide")
	class UAnimMontage* SlidingMontage;

	UPROPERTY(EditDefaultsOnly, Category = "PlayerCharacterMovement|Slide")
	float SlidingCapsuleHalfHeight = 45.f;

	float WalkingCapsuleHalfHeight = 0.f;

	// Used both for climbing and wall running
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerCharacterMovement")
	class UWallDetectorComponent* WallDetector;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float LineOfSight = 500.f;

public:
	APlayerCharacter(const FObjectInitializer& ObjectInitializer);
	virtual void Tick(float DeltaTime) override;

	class UPlayerCharacterMovementComponent* GetPlayerCharacterMovement() const;
	virtual FGenericTeamId GetGenericTeamId() const override;

	// Camera movement
	void Look(const FVector2D& Value);

	// Base character movement
	void Move(const FVector2D& Value);

	virtual void Jump() override;

	void Slide();
	float GetSlidingCapsuleHalfHeight() const;
	float GetWalkingCapsuleHalfHeight() const;

	void Interact();
	void TraceLineOfSight();
	UPROPERTY()
	TScriptInterface<class IInteractiveInterface> ObjectOnLineOfSight;

	FOnObjectOnLineOfSightFound OnObjectOnLineOfSightFound;
};
