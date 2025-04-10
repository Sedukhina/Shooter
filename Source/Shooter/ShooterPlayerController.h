// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputActionValue.h"
#include "ShooterPlayerController.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class SHOOTER_API AShooterPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:

	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	virtual void SetPawn(APawn* InPawn) override;

protected:
	// Possesed base character ptr
	TWeakObjectPtr<class APlayerCharacter> CachedCharacter;

	// Input Mapping
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	class UInputMappingContext* MappingContext;

	// Input Actions 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	class UInputAction* MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	class UInputAction* JumpAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	class UInputAction* SprintAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	class UInputAction* SlideAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	class UInputAction* AttackAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	class UInputAction* ReloadAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	class UInputAction* EquipNextItemAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	class UInputAction* EquipPreviousItemAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	class UInputAction* LookAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	class UInputAction* InteractAction;

	// Character functions
	void Move(const FInputActionValue& Value);
	void Jump();
	void StartSprinting();
	void StopSprinting();
	void Slide();
	void Attack();
	void Reload();
	void EquipNextItem();
	void EquipPreviousItem();

	void Look(const FInputActionValue& Value);
	void Interact();

	void OnObjectOnLineOfSightFound(UInputAction* Action);

	void CreateAndInitializeWidgets();
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widgets")
	TSubclassOf<class UPlayerHUD> PlayerHudWidgetClass;
	class UPlayerHUD* PlayerHUDWidget = nullptr;
};
