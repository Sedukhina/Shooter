// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PatrollingComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SHOOTER_API UPatrollingComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPatrollingComponent();

	bool CanPatrol() const;
	class APatrollingPath* GetPatrolPath() const;
	void SetPatrolPath(class APatrollingPath* Path);
	FVector SelectClosestWaypoint();
	FVector SelectNextWaypoint();

protected:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Path")
	class APatrollingPath* PatrollingPath;

private:
	int32 CurrentWayPointIndex = -1;
		
};
