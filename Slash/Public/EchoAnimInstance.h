// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CharacterState.h"
#include "EchoAnimInstance.generated.h"

class AMainCharacter;
/**
 * 
 */
UCLASS()
class SLASH_API UEchoAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
private:
	AMainCharacter* MainCharacter;
protected:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	UPROPERTY(BlueprintReadOnly)
	ECharacterEquipmentState State = ECharacterEquipmentState::ECS_UneuippedItem;
	UPROPERTY(BlueprintReadOnly)
	ECharacterAttackState AttackState = ECharacterAttackState::ECAS_NoAttacking;
	UPROPERTY(BlueprintReadOnly)
	ECharacterArmState ArmState = ECharacterArmState::ECAS_Armed;

};
