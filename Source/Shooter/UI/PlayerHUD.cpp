// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerHUD.h"

#include "Kismet/GameplayStatics.h"

#include "Characters/PlayerCharacter.h"
#include "Characters/Components/StatsComponent.h"

#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include <Characters/Components/CharacterEquipmentComponent.h>

#include "UI/InteractionAviableWidget.h"

void UPlayerHUD::NativeConstruct()
{
	Super::NativeConstruct();

	CachedPlayerCharacter = StaticCast<APlayerCharacter*>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	InteractionKey->SetVisibility(ESlateVisibility::Hidden);
}

void UPlayerHUD::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	UStatsComponent* PlayerStats = CachedPlayerCharacter->GetStatsComponent();

	int CurrentHealth = PlayerStats->GetCurrentHealth();
	CurrentHealthTextBlock->SetText(FText::FromString(FString::Printf(TEXT("%d"), CurrentHealth)));

	HealthBar->SetPercent(PlayerStats->GetCurrentHealthPercent());
	StaminaBar->SetPercent(PlayerStats->GetCurrentStaminaPercent());

	UCharacterEquipmentComponent* PlayerCharacterEquipmentComponent = CachedPlayerCharacter->GetCharacterEquipmentComponent();

	CurrentAmmoTextBlock->SetText(FText::FromString(FString::Printf(TEXT("%d"), PlayerCharacterEquipmentComponent->GetEquippedWeaponCurrentAmmo())));
	MaxAmmoTextBlock->SetText(FText::FromString(FString::Printf(TEXT("%d"), PlayerCharacterEquipmentComponent->GetEquippedWeaponMaxAmmo())));
	CurrentAmmoInInventory->SetText(FText::FromString(FString::Printf(TEXT("%d"), PlayerCharacterEquipmentComponent->GetEquippedWeaponInventoryAmmo())));
}

void UPlayerHUD::SetInteractionAviableWidgetVisibility(bool bIsVisible)
{
	if (bIsVisible)
	{
		InteractionKey->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		InteractionKey->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UPlayerHUD::SetInteractionAviableWidgetKeyName(FName Name)
{
	InteractionKey->SetKeyName(Name);
}
