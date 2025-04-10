// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "BaseCharacterAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTER_API UBaseCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
protected:
	virtual void NativeBeginPlay() override;

	TWeakObjectPtr<class ABaseCharacter> CachedOwner;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float Speed = 0.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float Direction = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bIsFalling = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FRotator AimRotation = FRotator::ZeroRotator;

public:
	virtual void NativeUpdateAnimation(float DeltaTime) override;
};
