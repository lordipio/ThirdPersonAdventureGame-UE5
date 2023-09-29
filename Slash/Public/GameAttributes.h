// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GameAttributes.generated.h"

/**
 * 
 */
UCLASS()
class SLASH_API UGameAttributes : public UGameInstance
{
	GENERATED_BODY()
	
public:

	FORCEINLINE void SetCoinNumber(int Value) { CoinNumber = Value; }
	FORCEINLINE void SetSoulNumber(int Value) { SoulNumber = Value; }

	FORCEINLINE int GetCoinNumber() { return CoinNumber; }
	FORCEINLINE int GetSoulNumber() { return SoulNumber; }

private:
	UPROPERTY()
	int32 CoinNumber = 0;
	UPROPERTY()
	int32 SoulNumber = 0;
};
