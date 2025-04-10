// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StatsComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnDeath, AActor*)
DECLARE_MULTICAST_DELEGATE(FOnOutOfStamina)

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SHOOTER_API UStatsComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UStatsComponent();

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	TWeakObjectPtr<class ABaseCharacter> CachedOwner;

	UPROPERTY(EditDefaultsOnly, Category = Health)
	float MaxHealth = 900;

	UPROPERTY(EditDefaultsOnly, Category = Health)
	float HealthRecoveryVelocity = 2;

	float CurrentHealth;

	UPROPERTY(EditDefaultsOnly, Category = Stamina)
	float MaxStamina = 900;

	UPROPERTY(EditDefaultsOnly, Category = Stamina)
	float StaminaRecoveryVelocity = 1;

	UPROPERTY(EditDefaultsOnly, Category = Stamina)
	float StaminaSprintConsumptionVelocity = 2.f;

	UPROPERTY(EditDefaultsOnly, Category = Stamina, meta = (ClampMin = "0.05", ClampMax = "1", UIMin = "0.05", UIMax = "1"))
	float OutOfStaminaEndThreshold = 0.3f;
	// Calculates in Begin Play for better performance
	float OutOfStaminaEndThresholdAbsolute;

	float CurrentStamina;
	bool bIsOutOfStamina = false;

public:
	float GetCurrentHealth();
	float GetCurrentHealthPercent();
	// Returns health > 0
	bool IsAlive();

	float GetMaxStamina();
	float GetCurrentStamina();
	float GetCurrentStaminaPercent();
	const bool IsOutOfStamina();

	UFUNCTION()
	void OnTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	FOnDeath OnDeath;
	FOnOutOfStamina OnOutOfStamina;
};
