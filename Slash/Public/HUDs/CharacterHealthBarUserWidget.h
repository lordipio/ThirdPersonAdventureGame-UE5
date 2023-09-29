// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "CharacterHealthBarUserWidget.generated.h"

/**
 * 
 */
class UProgressBar;
UCLASS()
class SLASH_API UCharacterHealthBarUserWidget : public UUserWidget
{
	GENERATED_BODY()
protected:
	
	UPROPERTY(meta = (BindWidget))
	UProgressBar* HealthBar;

public: 
	FORCEINLINE void SetHealth(float HealthBarPercentage)
	{
		if (HealthBar)
			HealthBar->SetPercent(HealthBarPercentage);
	}
};
