// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractiveInterface.generated.h"

UINTERFACE(MinimalAPI)
class UInteractiveInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SHOOTER_API IInteractiveInterface
{
	GENERATED_BODY()

public:

	virtual void Interact(class ABaseCharacter* Character) PURE_VIRTUAL(IInteractable::Interact, );
	virtual class UInputAction* GetInputAction() const PURE_VIRTUAL(IInteractable::GetActionEventName(), return nullptr;);
};
