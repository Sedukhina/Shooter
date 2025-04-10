// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InteractionAviableWidget.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTER_API UInteractionAviableWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void SetKeyName(FName KeyName);

protected:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* KeyNameTextBlock;

};
