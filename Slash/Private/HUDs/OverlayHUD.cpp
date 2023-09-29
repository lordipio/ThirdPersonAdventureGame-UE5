// Fill out your copyright notice in the Description page of Project Settings.

#include "Blueprint/UserWidget.h"
#include "HealthOverlay.h"
#include "HUDs/OverlayHUD.h"


AOverlayHUD::AOverlayHUD()
{

}

void AOverlayHUD::BeginPlay()
{
	Super::BeginPlay();

	// UserWidgetClass
	// CreateWidget

	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController)
		HealthOverlayWidget = CreateWidget<UHealthOverlay>(PlayerController, OverlayWidgetClass);

	if (HealthOverlayWidget)
		HealthOverlayWidget->AddToViewport();
	

}
