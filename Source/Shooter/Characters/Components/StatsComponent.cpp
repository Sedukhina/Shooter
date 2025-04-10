// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Components/StatsComponent.h"

#include "Characters/BaseCharacter.h"
#include "Characters/Components/BaseCharacterMovementComponent.h"

// Sets default values for this component's properties
UStatsComponent::UStatsComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UStatsComponent::BeginPlay()
{
	Super::BeginPlay();

	CurrentHealth = MaxHealth;
	CurrentStamina = MaxStamina;
	OutOfStaminaEndThresholdAbsolute = MaxStamina * OutOfStaminaEndThreshold;

	CachedOwner = StaticCast<ABaseCharacter*>(GetOwner());
	CachedOwner->OnTakeAnyDamage.AddDynamic(this, &UStatsComponent::OnTakeAnyDamage);
}

void UStatsComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!IsAlive())
	{
		return;
	}

	CurrentHealth += HealthRecoveryVelocity * DeltaTime;
	CurrentHealth = FMath::Clamp(CurrentHealth, 0, MaxHealth);

	UBaseCharacterMovementComponent* OwnerMovement = CachedOwner->GetBaseCharacterMovementComponent();
	if (bIsOutOfStamina)
	{
		CurrentStamina += StaminaRecoveryVelocity;
		if (CurrentStamina > OutOfStaminaEndThresholdAbsolute)
		{
			bIsOutOfStamina = false;
		}
	}
	else if (OwnerMovement->IsSprinting() && OwnerMovement->Velocity.Size() > 0)
	{
		CurrentStamina -= StaminaSprintConsumptionVelocity;
		CurrentStamina = FMath::Clamp(CurrentStamina, 0, MaxStamina);
		if (CurrentStamina == 0)
		{
			bIsOutOfStamina = true;
			OnOutOfStamina.Broadcast();
		}
	}
	else
	{
		CurrentStamina += StaminaRecoveryVelocity;
		CurrentStamina = FMath::Clamp(CurrentStamina, 0, MaxStamina);
	}
}

float UStatsComponent::GetCurrentHealth()
{
	return CurrentHealth;
}

float UStatsComponent::GetCurrentHealthPercent()
{
	return CurrentHealth / MaxHealth;
}

bool UStatsComponent::IsAlive()
{
	return CurrentHealth > 0 ? true : false;
}

float UStatsComponent::GetCurrentStamina()
{
	return CurrentStamina;
}

float UStatsComponent::GetMaxStamina()
{
	return MaxStamina;
}

float UStatsComponent::GetCurrentStaminaPercent()
{
	return GetCurrentStamina()/MaxStamina;
}

const bool UStatsComponent::IsOutOfStamina()
{
	return bIsOutOfStamina;
}

void UStatsComponent::OnTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	CurrentHealth -= Damage;
	CurrentHealth = FMath::Clamp(CurrentHealth, 0, MaxHealth);

	if (CurrentHealth == 0)
	{
		OnDeath.Broadcast(DamageCauser);
	}
}


