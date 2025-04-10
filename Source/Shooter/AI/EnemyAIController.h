// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyAIController.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTER_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()
	
public:

	AEnemyAIController();
	void SetupPatrolling();

	virtual void SetPawn(APawn* InPawn) override;
	virtual void ActorsPerceptionUpdated(const TArray<AActor*>& UpdatedActors);
	virtual void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UAIPerceptionComponent* AIPerception;

	TWeakObjectPtr<class ABaseEnemyCharacter> CachedPossesedCharacter;

	AActor* GetClosestSensedActor(TSubclassOf<class UAISense> SenseClass) const;

	// Patrolling
	bool bIsPatrolling = false;
	void TryMoveToNextTarget();
};
