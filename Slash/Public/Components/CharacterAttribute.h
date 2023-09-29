// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CharacterAttribute.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SLASH_API UCharacterAttribute : public UActorComponent
{
	GENERATED_BODY()
public:	
	// Sets default values for this component's properties
	UCharacterAttribute();
	
public:
	// Called when the game starts
	//virtual void BeginPlay() override;
	
	UPROPERTY(EditDefaultsOnly)
	float HealthPercentage = 1.f;

	UPROPERTY(EditDefaultsOnly)
	float MaxHealth = 1.f;




};

