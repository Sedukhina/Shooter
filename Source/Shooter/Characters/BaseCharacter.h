// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include <ShooterTypes.h>
#include "BaseCharacter.generated.h"

UCLASS(NotBlueprintable, Abstract)
class SHOOTER_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseCharacter(const FObjectInitializer& ObjectInitializer);

	class UBaseCharacterMovementComponent* GetBaseCharacterMovementComponent() const;
	class UCharacterEquipmentComponent* GetCharacterEquipmentComponent() const;
	class UStatsComponent* GetStatsComponent() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Called when character dies
	virtual void OnDeath(AActor* DeathCauser);

	virtual void Falling() override;
	virtual void NotifyJumpApex() override;
	virtual void Landed(const FHitResult& Hit) override;

	virtual void Attack();
	virtual void Reload();
	virtual void EquipNextItem();
	virtual void EquipPreviousItem();

	// Sprint
	void StartSprinting();
	void StopSprinting();
	void TryChangeSprintState();

protected:

	// Custom Movement Component
	 class UBaseCharacterMovementComponent* BaseCharacterMovementComponent;

	FVector FallingApex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCurveFloat* FallDamageCurve;

	bool bSprintRequested = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UStatsComponent* StatsComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UCharacterEquipmentComponent* CharacterEquipmentComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Faction")
	EFactions Faction = EFactions::Enemy;

};
