// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/InteractionAviableWidget.h"

#include "Components/TextBlock.h"

void UInteractionAviableWidget::SetKeyName(FName KeyName)
{
	KeyNameTextBlock->SetText(FText::FromName(KeyName));
}
