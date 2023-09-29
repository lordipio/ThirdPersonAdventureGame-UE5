// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IHitInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UIHitInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SLASH_API IIHitInterface
{
	GENERATED_BODY()
	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintNativeEvent)
	void GetHit(const FVector& HitLocation);

	virtual void SpawnParticle(UWorld* MyWorld, UParticleSystem* HitParticle, FVector SpawnPoint);

	// UENUM(BlueprintType)
	enum class ECharacterHitDirection : uint8
	{
		ECHD_Right,
		ECHD_Left,
		ECHD_Forward,
		ECHD_Backward
	};
};


