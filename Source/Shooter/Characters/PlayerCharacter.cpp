// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/PlayerCharacter.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include <Components/CapsuleComponent.h>
#include <Actors/InteractiveInterface.h>
#include "Components/PlayerCharacterMovementComponent.h"
#include "Components/WallDetectorComponent.h"
#include "Curves/CurveVector.h"

APlayerCharacter::APlayerCharacter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer.SetDefaultSubobjectClass<UPlayerCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("Camera Boom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 150.0f;
	CameraBoom->bUsePawnControlRotation = true;
	CameraBoom->SocketOffset = FVector(0, 70, 0);

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->AttachToComponent(CameraBoom, FAttachmentTransformRules::KeepRelativeTransform);
	FollowCamera->bUsePawnControlRotation = false;
	FollowCamera->SetRelativeLocation(FVector(0.f, 0.f, 100.f));

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	Faction = EFactions::Player;

	PlayerCharacterMovementComponent = StaticCast<UPlayerCharacterMovementComponent*>(GetCharacterMovement());
	WalkingCapsuleHalfHeight = GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight();

	WallDetector = CreateDefaultSubobject<UWallDetectorComponent>(TEXT("Wall Detector"));
}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	TraceLineOfSight();
}

UPlayerCharacterMovementComponent* APlayerCharacter::GetPlayerCharacterMovement() const
{
	return PlayerCharacterMovementComponent;
}

FGenericTeamId APlayerCharacter::GetGenericTeamId() const
{
	return (uint8)Faction;
}

void APlayerCharacter::Look(const FVector2D& Value)
{
	if (Controller && PlayerCharacterMovementComponent->IsSliding() == false)
	{
		AddControllerYawInput(Value.X);
		AddControllerPitchInput(Value.Y);
	}
}

void APlayerCharacter::Move(const FVector2D& Value)
{
	if (Controller)
	{
		if (PlayerCharacterMovementComponent->MovementMode == MOVE_Walking)
		{
			const FRotator Rotation = Controller->GetControlRotation();
			const FRotationMatrix YawRotationMatrix(FRotator(0, Rotation.Yaw, 0));

			AddMovementInput(YawRotationMatrix.GetUnitAxis(EAxis::X), Value.Y);
			AddMovementInput(YawRotationMatrix.GetUnitAxis(EAxis::Y), Value.X);
		}
		else
		{
			AddMovementInput(PlayerCharacterMovementComponent->GetCurrentWallUpVector(), Value.Y);
			AddMovementInput(PlayerCharacterMovementComponent->GetCurrentWallLeftVector(), Value.X);
		}
	}
}

void APlayerCharacter::Jump()
{
	if (PlayerCharacterMovementComponent->IsClimbing())
	{
		PlayerCharacterMovementComponent->DetachFromWall();
		return;
	}

	FWall Wall;
	if (WallDetector->DetectWall(Wall))
	{
		PlayerCharacterMovementComponent->AttachToWall(Wall);
	}
	else
	{
		Super::Jump();
	}
}

void APlayerCharacter::Slide()
{
	FSlideParams SlideParams;
	if (PlayerCharacterMovementComponent->CanSlide(SlideParams))
	{	
		SlideParams.SlidingDuratiuon = GetMesh()->GetAnimInstance()->Montage_Play(SlidingMontage, 1.0f, EMontagePlayReturnType::Duration, 0.f);
		PlayerCharacterMovementComponent->StartSliding(SlideParams);
	}
}

bool APlayerCharacter::CanJumpInternal_Implementation() const
{
	return (!PlayerCharacterMovementComponent->IsSliding()) && Super::CanJumpInternal_Implementation();
}

float APlayerCharacter::GetSlidingCapsuleHalfHeight() const
{
	return SlidingCapsuleHalfHeight;
}

float APlayerCharacter::GetWalkingCapsuleHalfHeight() const
{
	return WalkingCapsuleHalfHeight;
}

void APlayerCharacter::Interact()
{
	if (ObjectOnLineOfSight.GetInterface())
	{
		ObjectOnLineOfSight->Interact(this);
	}
}

void APlayerCharacter::TraceLineOfSight()
{
	FVector ViewLocation;
	FRotator ViewDirection;
	APlayerController* PlayerController = GetController<APlayerController>();
	Controller->GetPlayerViewPoint(ViewLocation, ViewDirection);
	
	FVector TraceEnd = ViewLocation + ViewDirection.Vector() * LineOfSight;
	FHitResult HitResult;
	if (GetWorld()->LineTraceSingleByChannel(HitResult, ViewLocation, TraceEnd, ECC_Visibility))
	{
		if (ObjectOnLineOfSight != HitResult.GetActor())
		{
			ObjectOnLineOfSight = HitResult.GetActor();
			UInputAction* Action = nullptr;
			if (ObjectOnLineOfSight.GetInterface())
			{
				Action = ObjectOnLineOfSight->GetInputAction();
			}
			OnObjectOnLineOfSightFound.ExecuteIfBound(Action);
		}
	}
}

