#pragma once

// Trace Channels
#define ECC_Bullet ECC_GameTraceChannel1
#define ECC_Climbing ECC_GameTraceChannel2

// AI Blackboard
const FName BB_Target = FName("Target");
const FName BB_NextLocation = FName("NextLocation");

// Sockets names
const FName CharacterRightArmItemSocket = FName("RightArmItemSocket");
const FName CharacterUnequippedItemSocket = FName("UnequippedItemSocket");
const FName WeaponMuzzleSocket = FName("MuzzleSocket");

// VFX params
const FName FXParamBulletTraceEnd = FName("TraceEnd");

UENUM(BlueprintType)
enum class EFactions : uint8
{
	Player UMETA(DisplayName = "Player"),
	Enemy UMETA(DisplayName = "Enemy")
};

UENUM(BlueprintType)
enum class EEquipmentSlots : uint8
{
	None,
	SideArm,
	PrimaryWeapon,
	MAX UMETA(Hidden)
};

UENUM(BlueprintType)
enum class EEquipableItemType : uint8
{
	None,
	Pistol
};

UENUM(BlueprintType)
enum class EAmmoType : uint8
{
	None,
	TracePistol, 
	Energy,
	MAX UMETA(Hidden)
};