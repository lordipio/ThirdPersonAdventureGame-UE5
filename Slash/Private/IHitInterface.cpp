// Fill out your copyright notice in the Description page of Project Settings.

#include "IHitInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"

// Add default functionality here for any IIHitInterface functions that are not pure virtual.


void IIHitInterface::SpawnParticle(UWorld* MyWorld, UParticleSystem* HitParticle, FVector SpawnPoint)
{
	UGameplayStatics::SpawnEmitterAtLocation(MyWorld, HitParticle, SpawnPoint);
}


