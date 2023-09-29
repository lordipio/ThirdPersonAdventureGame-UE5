// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "Weapon.generated.h"

/**
 * 
 */
class UBoxComponent;
UCLASS()
class SLASH_API AWeapon : public AItem
{
	GENERATED_BODY()
		AWeapon();
protected:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void EventBeginOverlapFunction(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	virtual void EventEndOverlapFunction(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UBoxComponent* WeaponBoxComponent = NULL;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USceneComponent* WeaponStartTrace = NULL;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USceneComponent* WeaponEndTrace = NULL;

	FHitResult Hit;

	UFUNCTION()
	virtual void BoxComponentBeginOverlap( UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult );

	TArray<AActor*> BoxTraceIgnoredActors;

	UFUNCTION(BlueprintImplementableEvent)
	void CreateField(const FVector& HitPoint);

public:
	void SetWeaponBoxCollision(ECollisionEnabled::Type CollisionType);
private:
	TArray<AActor*> IgnoredActors;
	bool bReturner = false;

};
