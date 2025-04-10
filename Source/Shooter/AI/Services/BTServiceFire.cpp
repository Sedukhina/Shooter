// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Services/BTServiceFire.h"
#include <BehaviorTree/BlackboardComponent.h>
#include "Characters/Components/CharacterEquipmentComponent.h"
#include "AIController.h"
#include <Characters/BaseCharacter.h>

UBTServiceFire::UBTServiceFire()
{
}

void UBTServiceFire::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AAIController* AIController = OwnerComp.GetAIOwner();
	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	if (!IsValid(AIController) || !IsValid(Blackboard))
	{
		return;
	}

	ABaseCharacter* Character = Cast<ABaseCharacter>(AIController->GetPawn());

	AActor* CurrentTarget = Cast<AActor>(Blackboard->GetValueAsObject(TargetKey.SelectedKeyName));
	if (!IsValid(CurrentTarget))
	{
		return;
	}

	const UCharacterEquipmentComponent* EquipmentComponent = Character->GetCharacterEquipmentComponent();
	if (!(EquipmentComponent->IsReloading()))
	{
		Character->Attack();
	}
}
