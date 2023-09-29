#pragma once

UENUM(BlueprintType)
enum class ECharacterEquipmentState : uint8
{
	ECS_UneuippedItem UMETA(DisplayName = "Unequipped Item"),
	ECS_EquippedItem UMETA(DisplayName = "Equipped Item"),
};

UENUM(BlueprintType)
enum class ECharacterAttackState : uint8
{
	ECAS_Attacking UMETA(DisplayName = "Attacking"),
	ECAS_NoAttacking UMETA(DisplayName = "No Attacking")
};

UENUM(BlueprintType)
enum class ECharacterArmState : uint8
{
	ECAS_Armed UMETA(DisplayName = "Armed"),
	ECAS_Unarmed UMETA(DisplayName = "Unarmed")
};

UENUM(BlueprintType)
enum class ECharacterMovementState : uint8
{
	ECAS_Move UMETA(DisplayName = "Move"),
	ECAS_NoMovement UMETA(DisplayName = "No Movement")
};

UENUM(BlueprintType)
enum class ECharacterRollState : uint8
{
	ECAS_Rolling UMETA(DisplayName = "Rolling"),
	ECAS_NotRolling UMETA(DisplayName = "Not Rolling")
};

UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	EES_Attack UMETA(DisplayName = "Attack"),
	EES_Patrol UMETA(DisplayName = "Patrol"),
	EES_Chase UMETA(DisplayName = "Chase")
};

UMETA(BlueprintType)
enum class ECharacterHealth : uint8
{
	ECH_Alive UMETA(DisplayName = "Alive"),
	ECH_Died UMETA(DisplayName = "Died")
};
















