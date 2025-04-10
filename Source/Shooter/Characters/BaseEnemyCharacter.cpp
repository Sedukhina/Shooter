// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/BaseEnemyCharacter.h"
#include "BaseEnemyCharacter.h"
#include "AIController.h"
#include "Characters/BaseCharacter.h"
#include "Characters/Components/AI/PatrollingComponent.h"

ABaseEnemyCharacter::ABaseEnemyCharacter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PatrolingComponent = CreateDefaultSubobject<UPatrollingComponent>(TEXT("Patrolling"));
}

void ABaseEnemyCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	AAIController* NewAIController = Cast<AAIController>(NewController);
	if (IsValid(NewAIController))
	{
		NewAIController->SetGenericTeamId((uint8)Faction);
	}
}

UBehaviorTree* ABaseEnemyCharacter::GetBehaviorTree() const
{
	return BehaviorTree;
}

UPatrollingComponent* ABaseEnemyCharacter::GetPatrollingComponent() const
{
	return PatrolingComponent;
}

void ABaseEnemyCharacter::OnDeath(AActor* DeathCauser)
{
	Super::OnDeath(DeathCauser);
	GetWorld()->GetTimerManager().SetTimer(DeathToDestroy, this, &ABaseEnemyCharacter::DestroyAfterDeath, TimeBeforeDestroymentAfterDeath);
}

void ABaseEnemyCharacter::DestroyAfterDeath()
{
	OnDestroyAI.Broadcast(GetPatrollingComponent()->GetPatrolPath());
	Destroy();
}
