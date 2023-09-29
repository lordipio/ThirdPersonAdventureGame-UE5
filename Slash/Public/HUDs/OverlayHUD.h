// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "GameFramework/HUD.h"

#include "OverlayHUD.generated.h"

class UHealthOverlay;
UCLASS()
class SLASH_API AOverlayHUD : public AHUD
{
	GENERATED_BODY()

	AOverlayHUD();

	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> OverlayWidgetClass;

	UPROPERTY()
	UHealthOverlay* HealthOverlayWidget = NULL;
public:
	
	FORCEINLINE UHealthOverlay* GetOverlayWidget() { return HealthOverlayWidget; }
};
