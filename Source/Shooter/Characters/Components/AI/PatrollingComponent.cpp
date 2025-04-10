// Fill out your copyright notice in the Description page of Project Settings.

#include "PatrollingComponent.h"
#include "Actors/AI/PatrollingPath.h"

// Sets default values for this component's properties
UPatrollingComponent::UPatrollingComponent()
{

}

bool UPatrollingComponent::CanPatrol() const
{
	return IsValid(PatrollingPath) && PatrollingPath->GetWaypoints().Num() > 0;
}

APatrollingPath* UPatrollingComponent::GetPatrolPath() const
{
	return PatrollingPath;
}

void UPatrollingComponent::SetPatrolPath(APatrollingPath* Path)
{
	if (IsValid(Path))
	{
		PatrollingPath = Path;
	}
}

FVector UPatrollingComponent::SelectClosestWaypoint()
{
	FVector OwnerLocation = GetOwner()->GetActorLocation();
	const TArray<FVector> WayPoints = PatrollingPath->GetWaypoints();
	FTransform PathTransform = PatrollingPath->GetActorTransform();

	FVector ClosestWayPoint;
	float MinSqDistance = FLT_MAX;
	for (int32 i = 0; i < WayPoints.Num(); ++i)
	{
		FVector WayPointWorld = PathTransform.TransformPosition(WayPoints[i]);
		float CurrentSqDistance = (OwnerLocation - WayPointWorld).SizeSquared();
		if (CurrentSqDistance < MinSqDistance)
		{
			MinSqDistance = CurrentSqDistance;
			ClosestWayPoint = WayPointWorld;
			CurrentWayPointIndex = i;
		}
	}
	return ClosestWayPoint;
}

FVector UPatrollingComponent::SelectNextWaypoint()
{
	++CurrentWayPointIndex;
	const TArray<FVector> WayPoints = PatrollingPath->GetWaypoints();
	if (CurrentWayPointIndex == PatrollingPath->GetWaypoints().Num())
	{
		CurrentWayPointIndex = 0;
	}
	FTransform PathTransform = PatrollingPath->GetActorTransform();
	FVector WayPoint = PathTransform.TransformPosition(WayPoints[CurrentWayPointIndex]);
	return WayPoint;
}


