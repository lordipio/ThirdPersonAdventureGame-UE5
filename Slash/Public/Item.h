// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemState.h"
#include "Item.generated.h"

class USphereComponent;
class UNiagaraComponent;
class AMainCharacter;
UCLASS()
class SLASH_API AItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItem();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* MeshRepresenter;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* SceneComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USphereComponent* SphereComponent;

	UFUNCTION(BlueprintCallable)
	FORCEINLINE EItemState GetItemState () const{ return CurrentItemState; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetItemState(bool bIsAcquired) 
	{ 
		if (bIsAcquired) 
			CurrentItemState = EItemState::EIS_Acquired;
		else
			CurrentItemState = EItemState::EIS_NonAcquired;
	}


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UFUNCTION()
	virtual void EventBeginOverlapFunction(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	virtual void EventEndOverlapFunction(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsHover = true;

	//UPROPERTY(BlueprintReadOnly)
	AMainCharacter* CurrentMainCharacter = NULL;
	AMainCharacter* MainCharacter = NULL;

	FORCEINLINE void RotateActor(float DeltaTime) { AddActorWorldRotation(FRotator(0.f, DeltaTime * 50.f, 0.f)); };
	void HoverActor(float DeltaTime);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UNiagaraComponent* HoverParticle = NULL;

	UFUNCTION()
		virtual void Collided(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	EItemState CurrentItemState = EItemState::EIS_NonAcquired;
	float StopWatch = 0.f;
	FVector SinMovementOffset = FVector::Zero();

	class AOverlayHUD* MyHUD;
	class UHealthOverlay* MyWidget;


protected:
	void AddCoinsNumber(int Value);
	void AddSoulsNumber(int Value);

	void DestroyItem();
};
