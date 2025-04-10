// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <Actors/InteractiveInterface.h>
#include <Components/TimelineComponent.h>
#include "Door.generated.h"


UCLASS()
class SHOOTER_API ADoor : public AActor, public IInteractiveInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADoor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	class UInputAction* InteractAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Door")
	UStaticMeshComponent* DoorMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Door")
	USceneComponent* DoorPivot;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Door")
	float ClosedDoorAngle = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Door")
	float OpenDoorAngle = 90.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Door")
	UCurveFloat* DoorOpeningAnimCurve;

	FTimeline DoorOpenAnimTimeline;
	UFUNCTION()
	void UpdateDoorAnimation(float Alpha);
	UFUNCTION()
	void OnDoorAnimationFinished();

	bool bIsOpened = false;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void Interact(class ABaseCharacter* Character) override;
	virtual class UInputAction* GetInputAction() const override;
};
