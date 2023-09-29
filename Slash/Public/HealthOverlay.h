// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

#include "HealthOverlay.generated.h"

class UProgressBar;
class UCanvasPanel;
class UImage;
class UTextBlock;

UCLASS()
class SLASH_API UHealthOverlay : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* Canvas;

#pragma region ProgressBars

	UPROPERTY(meta = (BindWidget))
	UProgressBar* HealthBar;

	UPROPERTY(meta = (BindWidget))
	UProgressBar* StaminaBar;

#pragma endregion

#pragma region Images

	UPROPERTY(meta = (BindWidget))
	UImage* BarsOverlay;

	UPROPERTY(meta = (BindWidget))
	UImage* CoinIcon;

	UPROPERTY(meta = (BindWidget))
	UImage * SoulIcon;

	UPROPERTY(meta = (BindWidget))
	UImage* AvatarIcon;

#pragma endregion

#pragma region TextBlocks

	UPROPERTY(meta = (BindWidget))
		UTextBlock* CoinNumber;

	UPROPERTY(meta = (BindWidget))
		UTextBlock* SoulNumber;
#pragma endregion

public:
#pragma region Setters

	FORCEINLINE void SetHealthBar(const float Value) const { HealthBar->SetPercent(Value); }
	FORCEINLINE void SetStaminaBar(const float Value)  const { StaminaBar->SetPercent(Value); }
	FORCEINLINE void SetCoinNumber(const int Value)  const { CoinNumber->SetText(FText::FromString(FString::Printf(TEXT("%i"), Value))); }
	FORCEINLINE void SetSoulNumber(const int Value)  const { SoulNumber->SetText(FText::FromString(FString::Printf(TEXT("%i"), Value))); }

#pragma endregion


};
