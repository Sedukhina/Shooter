// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/BaseCharacter.h"
#include "BaseEnemyCharacter.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnDestroyAI, class APatrollingPath*)
/**
 * 
 */
UCLASS(Blueprintable)
class SHOOTER_API ABaseEnemyCharacter : public ABaseCharacter
{
	GENERATED_BODY()
	
public:
	ABaseEnemyCharacter(const FObjectInitializer& ObjectInitializer);

	virtual void PossessedBy(AController* NewController) override;

	class UBehaviorTree* GetBehaviorTree() const;

	class UPatrollingComponent* GetPatrollingComponent() const;

	virtual void OnDeath(AActor* DeathCauser) override;
	FTimerHandle DeathToDestroy;
	float TimeBeforeDestroymentAfterDeath = 5.f;
	FOnDestroyAI OnDestroyAI;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
	class UBehaviorTree* BehaviorTree;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UPatrollingComponent* PatrolingComponent;

	UFUNCTION()
	void DestroyAfterDeath();
};
