// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapon.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Character.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "IHitInterface.h"
#include "Enemy.h"
#include "MainCharacter.h"

AWeapon::AWeapon()
{
	WeaponBoxComponent = CreateDefaultSubobject<UBoxComponent>(FName("Weapon Box Component"));
	WeaponBoxComponent->SetupAttachment(GetRootComponent());

	WeaponStartTrace = CreateDefaultSubobject<USceneComponent>(FName("Weapon Start Trace"));
	WeaponStartTrace->SetupAttachment(GetRootComponent());

	WeaponEndTrace = CreateDefaultSubobject<USceneComponent>(FName("Weapon End Trace"));
	WeaponEndTrace->SetupAttachment(GetRootComponent());
}


void AWeapon::BeginPlay()
{
	Super::BeginPlay();


	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::EventBeginOverlapFunction);
	SphereComponent->OnComponentEndOverlap.AddDynamic(this, &AWeapon::EventEndOverlapFunction);
	WeaponBoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::BoxComponentBeginOverlap);

	WeaponBoxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

}

void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void AWeapon::EventBeginOverlapFunction(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::EventBeginOverlapFunction(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
}

void AWeapon::EventEndOverlapFunction(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::EventEndOverlapFunction(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
}



void AWeapon::BoxComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	if (bReturner)
	{
		return;
	}

	if (OtherActor)
	{
		IgnoredActors.AddUnique(GetOwner());
		UE_LOG(LogTemp, Error, TEXT("Ignored!"))
	}

	bool bIsCollided = UKismetSystemLibrary::BoxTraceSingle(this, WeaponStartTrace->GetComponentLocation(),
		WeaponEndTrace->GetComponentLocation(),
		FVector(5.f, 5.f, 5.f),
		GetActorRotation(),
		ETraceTypeQuery::TraceTypeQuery1,
		false,
		IgnoredActors,
		EDrawDebugTrace::None,
		Hit,
		true, FLinearColor::Transparent, FLinearColor::Green, 100000.f);

	IIHitInterface* HitInterface = Cast<IIHitInterface>(OtherActor);


	if (HitInterface && Hit.GetActor())
	{
		HitInterface->Execute_GetHit(OtherActor, Hit.Location);
		CreateField(Hit.Location);

		APawn* OtherActorPawn = Cast<APawn>(OtherActor);
		if (OtherActorPawn)
			UGameplayStatics::ApplyDamage(OtherActor, 1.f, OtherActorPawn->GetController(), MainCharacter, UDamageType::StaticClass());
	}
	

}

void AWeapon::SetWeaponBoxCollision(ECollisionEnabled::Type CollisionType)
{
	if (WeaponBoxComponent)
		WeaponBoxComponent->SetCollisionEnabled(CollisionType);
	if (CollisionType == ECollisionEnabled::NoCollision)
	{
		IgnoredActors.Empty();
	}
}
