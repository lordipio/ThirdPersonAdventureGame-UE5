// Fill out your copyright notice in the Description page of Project Settings.

#include "EchoAnimInstance.h"
#include "MainCharacter.h"

void UEchoAnimInstance::NativeInitializeAnimation()
{
	MainCharacter = Cast<AMainCharacter>(TryGetPawnOwner());
	if (!MainCharacter)
		UE_LOG(LogTemp, Error, TEXT("Main Character is NULL"))
}

void UEchoAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	if (MainCharacter)
	{
		State = MainCharacter->GetCharacterState();
		AttackState = MainCharacter->GetCharacterAttackState();
		ArmState = MainCharacter->GetCharacterArmState();
		//UE_LOG(LogTemp, Error, TEXT("%i"), State)
	}
}
