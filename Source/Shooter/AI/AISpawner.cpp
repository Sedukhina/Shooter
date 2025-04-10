// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/AISpawner.h"
#include "Characters/Components/AI/PatrollingComponent.h"
#include "EnemyAIController.h"

// Sets default values
AAISpawner::AAISpawner()
{
	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("Spawner Root"));
	SetRootComponent(Root);
}

void AAISpawner::Spawn(APatrollingPath* Path)
{
	if (!bCanSpawn || !IsValid(SpawnedCharacterClass))
	{
		return;
	}

	ABaseEnemyCharacter* NewCharacter = GetWorld()->SpawnActor<ABaseEnemyCharacter>(SpawnedCharacterClass, GetTransform());
	if (!IsValid(NewCharacter->Controller))
	{
		NewCharacter->SpawnDefaultController();
	}
	NewCharacter->GetPatrollingComponent()->SetPatrolPath(Path);
	AEnemyAIController* Controller = Cast<AEnemyAIController>(NewCharacter->Controller);
	Controller->SetupPatrolling();
	NewCharacter->OnDestroyAI.AddUObject(this, &AAISpawner::Spawn);
}

// Called when the game starts or when spawned
void AAISpawner::BeginPlay()
{
	Super::BeginPlay();

	if (bSpawnOnStart)
	{
		for (const TPair<APatrollingPath*, int32>& Patrol : PatrollingPaths)
		{
			for (size_t i = 0; i < Patrol.Value; i++)
			{
				Spawn(Patrol.Key);
			}
		}
	}
}

