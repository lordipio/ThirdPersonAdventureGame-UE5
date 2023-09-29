// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "IHitInterface.h"
#include "BreakableObject.generated.h"

class ATreasure;
class UGeometryCollectionComponent;
UCLASS()
class SLASH_API ABreakableObject : public AActor, public IIHitInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABreakableObject();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<ATreasure>> Treasures;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UGeometryCollectionComponent* BreakabeObjectGeometryCollection = NULL;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UCapsuleComponent* CapsuleCollision = NULL;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	USceneComponent* BaseScene = NULL;

private:	
	bool bIsBroken = false;
	virtual void GetHit_Implementation(const FVector& HitLocation) override;


};
