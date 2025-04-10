// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterPlayerController.h"

#include "Characters/PlayerCharacter.h"
#include "UI/PlayerHUD.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"

void AShooterPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(MappingContext, 0);
	}
}

void AShooterPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// Checking if current input is enhanced input component 
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent))
	{
		// Binding actions to input
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AShooterPlayerController::Move);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AShooterPlayerController::Jump);
		EnhancedInputComponent->BindAction(SlideAction, ETriggerEvent::Triggered, this, &AShooterPlayerController::Slide);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AShooterPlayerController::Look);
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this, &AShooterPlayerController::Interact);
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &AShooterPlayerController::Attack);
		EnhancedInputComponent->BindAction(ReloadAction, ETriggerEvent::Triggered, this, &AShooterPlayerController::Reload);
		EnhancedInputComponent->BindAction(EquipNextItemAction, ETriggerEvent::Triggered, this, &AShooterPlayerController::EquipNextItem);
		EnhancedInputComponent->BindAction(EquipPreviousItemAction, ETriggerEvent::Triggered, this, &AShooterPlayerController::EquipPreviousItem);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Triggered, this, &AShooterPlayerController::StartSprinting);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &AShooterPlayerController::StopSprinting);
	}
}

void AShooterPlayerController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);
	CreateAndInitializeWidgets();

	CachedCharacter = Cast<APlayerCharacter>(GetPawn());
	if (CachedCharacter.IsValid())
	{
		CachedCharacter->OnObjectOnLineOfSightFound.BindUObject(this, &AShooterPlayerController::OnObjectOnLineOfSightFound);
	}
}

void AShooterPlayerController::Move(const FInputActionValue& Value)
{
	if (CachedCharacter.IsValid())
	{
		// Converting values so input system is completely separated from character
		FVector2D MovementInputVector = Value.Get<FVector2D>();
		CachedCharacter->Move(MovementInputVector);
	}
}

void AShooterPlayerController::Jump()
{
	if (CachedCharacter.IsValid())
	{
		CachedCharacter->Jump();
	}
}

void AShooterPlayerController::StartSprinting()
{
	if (CachedCharacter.IsValid())
	{
		CachedCharacter->StartSprinting();
	}
}

void AShooterPlayerController::StopSprinting()
{
	if (CachedCharacter.IsValid())
	{
		CachedCharacter->StopSprinting();
	}
}

void AShooterPlayerController::Slide()
{
	if (CachedCharacter.IsValid())
	{
		CachedCharacter->Slide();
	}
}

void AShooterPlayerController::Attack()
{
	if (CachedCharacter.IsValid())
	{
		CachedCharacter->Attack();
	}
}

void AShooterPlayerController::Reload()
{
	if (CachedCharacter.IsValid())
	{
		CachedCharacter->Reload();
	}
}

void AShooterPlayerController::EquipNextItem()
{
	if (CachedCharacter.IsValid())
	{
		CachedCharacter->EquipNextItem();
	}
}

void AShooterPlayerController::EquipPreviousItem()
{
	if (CachedCharacter.IsValid())
	{
		CachedCharacter->EquipPreviousItem();
	}
}

void AShooterPlayerController::Look(const FInputActionValue& Value)
{
	if (CachedCharacter.IsValid())
	{
		// Converting values so input system is completely separated from character
		FVector2D CameraMovementInputVector = Value.Get<FVector2D>();
		CachedCharacter->Look(CameraMovementInputVector);
	}
}

void AShooterPlayerController::Interact()
{
	if (CachedCharacter.IsValid())
	{
		CachedCharacter->Interact();
	}
}

void AShooterPlayerController::OnObjectOnLineOfSightFound(UInputAction* Action)
{
	if (IsValid(Action))
	{
		UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
		TArray<FKey> ActionKeys = Subsystem->QueryKeysMappedToAction(Action);
		const bool HasAnyKeys = ActionKeys.Num() != 0;
		if (HasAnyKeys)
		{
			FName ActionKey = ActionKeys[0].GetFName();
			PlayerHUDWidget->SetInteractionAviableWidgetKeyName(ActionKey);
		}
		PlayerHUDWidget->SetInteractionAviableWidgetVisibility(HasAnyKeys);
	}
	else
	{
		PlayerHUDWidget->SetInteractionAviableWidgetVisibility(false);
	}
}

void AShooterPlayerController::CreateAndInitializeWidgets()
{
	PlayerHUDWidget = CreateWidget<UPlayerHUD>(GetWorld(), PlayerHudWidgetClass);
	if (IsValid(PlayerHUDWidget))
	{
		PlayerHUDWidget->AddToViewport();
	}
	SetInputMode(FInputModeGameOnly{});
	bShowMouseCursor = false;
}
