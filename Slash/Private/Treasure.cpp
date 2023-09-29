// Fill out your copyright notice in the Description page of Project Settings.

#include "Treasure.h"
#include "NiagaraComponent.h"
#include "GameAttributes.h"
#include "Components/SphereComponent.h"

ATreasure::ATreasure()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ATreasure::BeginPlay()
{
	Super::BeginPlay();
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ATreasure::Collided);
}


void ATreasure::Collided(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag(FName("Player")))
	{
		AddCoinsNumber(10);
		HoverParticle->Deactivate();
		UE_LOG(LogTemp, Error, TEXT("Collided With Teasure!"))
		SetActorHiddenInGame(true);
		DestroyItem();
		// Super::Collided(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	}
}

