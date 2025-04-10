// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Door.h"
#include <ShooterTypes.h>

// Sets default values
ADoor::ADoor()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;

	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("Door Root"));
	SetRootComponent(Root);
	DoorPivot = CreateDefaultSubobject<USceneComponent>(TEXT("Door Pivot"));
	DoorPivot->SetupAttachment(GetRootComponent());
	DoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Door Mesh"));
	DoorMesh->SetupAttachment(DoorPivot);
}

// Called when the game starts or when spawned
void ADoor::BeginPlay()
{
	Super::BeginPlay();
	
	if (IsValid(DoorOpeningAnimCurve))
	{
		FOnTimelineFloatStatic DoorAnimDelegate;
		DoorAnimDelegate.BindUObject(this, &ADoor::UpdateDoorAnimation);
		DoorOpenAnimTimeline.AddInterpFloat(DoorOpeningAnimCurve, DoorAnimDelegate);

		FOnTimelineEventStatic DoorOpenedDelegate;
		DoorOpenedDelegate.BindUObject(this, &ADoor::OnDoorAnimationFinished);
		DoorOpenAnimTimeline.SetTimelineFinishedFunc(DoorOpenedDelegate);
	}
}

void ADoor::UpdateDoorAnimation(float Alpha)
{
	float YawAngle = FMath::Lerp(ClosedDoorAngle, OpenDoorAngle, FMath::Clamp(Alpha, 0.0f, 1.0f));
	DoorPivot->SetRelativeRotation(FRotator(0.0f, YawAngle, 0.0f));
}

void ADoor::OnDoorAnimationFinished()
{
	SetActorTickEnabled(false);
}

// Called every frame
void ADoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	DoorOpenAnimTimeline.TickTimeline(DeltaTime);
}

void ADoor::Interact(ABaseCharacter* Character)
{
	SetActorTickEnabled(true);
	if (bIsOpened)
	{
		DoorOpenAnimTimeline.Reverse();
	}
	else
	{
		DoorOpenAnimTimeline.Play();
	}
	bIsOpened = !bIsOpened;
}

UInputAction* ADoor::GetInputAction() const
{
	return InteractAction;
}

