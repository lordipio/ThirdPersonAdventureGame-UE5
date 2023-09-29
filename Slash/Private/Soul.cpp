// Fill out your copyright notice in the Description page of Project Settings.

#include "Soul.h"
#include "Components/CharacterAttribute.h"
#include "Components/SphereComponent.h"
#include "NiagaraComponent.h"

ASoul::ASoul()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ASoul::BeginPlay()
{
	Super::BeginPlay();
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ASoul::Collided);
}

void ASoul::Collided(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag(FName("Player")))
	{
		// UCharacterAttribute::SetSoulNumber(1);
		//UCharacterAttribute::CoinNumber = 10;
		AddSoulsNumber(10);
		// HoverParticle->Deactivate();
		// GetParentActor()->SetHidden(true);
		DestroyItem();
		//Super::Collided(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	}
}
