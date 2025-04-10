// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/Animation/BaseCharacterAnimInstance.h"
#include "PlayerCharacterAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTER_API UPlayerCharacterAnimInstance : public UBaseCharacterAnimInstance
{
	GENERATED_BODY()

protected:
	virtual void NativeBeginPlay() override;

	TWeakObjectPtr<class APlayerCharacter> CachedPlayerOwner;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bIsClimbing = false;
	
public:
	virtual void NativeUpdateAnimation(float DeltaTime) override;

};
