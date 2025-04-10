// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAIController.h"

#include "Perception/AIPerceptionComponent.h"
#include "Characters/BaseEnemyCharacter.h"
#include <BehaviorTree/BlackboardComponent.h>
#include "Characters/Components/AI/PatrollingComponent.h"
#include "Perception/AISense_Sight.h"
#include "Navigation/PathFollowingComponent.h"
#include "ShooterTypes.h"

AEnemyAIController::AEnemyAIController()
{
	AIPerception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AI Perception"));
}

void AEnemyAIController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);

	if (IsValid(InPawn))
	{
		checkf(InPawn->IsA<ABaseEnemyCharacter>(), TEXT("AEnemyAIController::SetPawn AVeilAIController can possess only ABaseEnemyCharacte"));
		CachedPossesedCharacter = StaticCast<ABaseEnemyCharacter*>(InPawn);
		RunBehaviorTree(CachedPossesedCharacter->GetBehaviorTree());
		SetupPatrolling();
	}
}

void AEnemyAIController::ActorsPerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
	Super::ActorsPerceptionUpdated(UpdatedActors);

	if (UpdatedActors[0] && Blackboard)
	{
		FString Str;
		Blackboard->SetValueAsObject(BB_Target, UpdatedActors[0]);
	}
	TryMoveToNextTarget();
}

void AEnemyAIController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	Super::OnMoveCompleted(RequestID, Result);
	if (!Result.IsSuccess())
	{
		return;
	}
	TryMoveToNextTarget();
}

AActor* AEnemyAIController::GetClosestSensedActor(TSubclassOf<UAISense> SenseClass) const
{
	if (!IsValid(GetPawn()))
	{
		return nullptr;
	}

	TArray<AActor*> SensedActors;
	PerceptionComponent->GetCurrentlyPerceivedActors(SenseClass, SensedActors);

	AActor* ClosestActor = nullptr;
	float MinSquaredDistance = FLT_MAX;
	FVector PawnLocation = GetPawn()->GetActorLocation();

	for (AActor* SeenActor : SensedActors)
	{
		float CurrentSquaredDistance = (PawnLocation - SeenActor->GetActorLocation()).SizeSquared();
		if (CurrentSquaredDistance < MinSquaredDistance)
		{
			MinSquaredDistance = CurrentSquaredDistance;
			ClosestActor = SeenActor;
		}
	}
	return ClosestActor;
}

void AEnemyAIController::SetupPatrolling()
{
	UPatrollingComponent* PatrollingComponent = CachedPossesedCharacter->GetPatrollingComponent();
	if (PatrollingComponent->CanPatrol())
	{
		FVector ClosestWayPoint = PatrollingComponent->SelectClosestWaypoint();
		if (IsValid(Blackboard))
		{
			Blackboard->SetValueAsVector(BB_NextLocation, ClosestWayPoint);
			Blackboard->SetValueAsObject(BB_Target, nullptr);
		}
		bIsPatrolling = true;
	}
}

void AEnemyAIController::TryMoveToNextTarget()
{
	UPatrollingComponent* PatrollingComponent = CachedPossesedCharacter->GetPatrollingComponent();
	AActor* ClosestActor = GetClosestSensedActor(UAISense_Sight::StaticClass());
	// If find target to chase - chasing
	if (IsValid(ClosestActor))
	{
		if (IsValid(Blackboard))
		{
			Blackboard->SetValueAsObject(BB_Target, ClosestActor);
			SetFocus(ClosestActor, EAIFocusPriority::Gameplay);
		}
		bIsPatrolling = false;
	}
	// If target to chasev not found - trying to patrol
	else if (PatrollingComponent->CanPatrol())
	{
		FVector WayPoint = bIsPatrolling ? PatrollingComponent->SelectNextWaypoint() : PatrollingComponent->SelectClosestWaypoint();
		if (IsValid(Blackboard))
		{
			ClearFocus(EAIFocusPriority::Gameplay);
			Blackboard->SetValueAsVector(BB_NextLocation, WayPoint);
			Blackboard->SetValueAsObject(BB_Target, nullptr);
		}
		bIsPatrolling = true;
	}
}
