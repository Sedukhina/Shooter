// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerHUD.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTER_API UPlayerHUD : public UUserWidget
{
	GENERATED_BODY()

protected:

	TWeakObjectPtr<class APlayerCharacter> CachedPlayerCharacter;

	virtual void NativeConstruct() override;
	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UProgressBar* HealthBar;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* CurrentHealthTextBlock;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UProgressBar* StaminaBar;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* CurrentAmmoTextBlock;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* MaxAmmoTextBlock;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* CurrentAmmoInInventory;

	UPROPERTY(meta = (BindWidget))
	class UInteractionAviableWidget* InteractionKey;

public:
	void SetInteractionAviableWidgetVisibility(bool bIsVisible);
	void SetInteractionAviableWidgetKeyName(FName Name);
};
