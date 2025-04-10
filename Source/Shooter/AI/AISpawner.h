// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <Characters/BaseEnemyCharacter.h>
#include "AISpawner.generated.h"


UCLASS()
class SHOOTER_API AAISpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAISpawner();

	UFUNCTION()
	void Spawn(class APatrollingPath* Path);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<ABaseEnemyCharacter> SpawnedCharacterClass;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
	// Map of paths and characters setted to it
	TMap<class APatrollingPath*, int32> PatrollingPaths;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bSpawnOnStart = true;

	bool bCanSpawn = true;
};
