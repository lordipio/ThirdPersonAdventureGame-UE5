// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "HUDs/CharacterHealthBarUserWidget.h"
#include "HealthBarWidgetComponent.generated.h"

/**
 * 
 */
// class UCharacterHealthBarUserWidget;
UCLASS()
class SLASH_API UHealthBarWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()

public:

	FORCEINLINE void SetHealthProgressBar(float Value)
	{
		UCharacterHealthBarUserWidget* HealthBarWidget = Cast<UCharacterHealthBarUserWidget>(GetUserWidgetObject());
		if (HealthBarWidget)
			HealthBarWidget->SetHealth(Value);
	}
	
};
