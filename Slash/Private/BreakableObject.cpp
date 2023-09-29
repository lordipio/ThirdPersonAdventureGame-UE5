// Fill out your copyright notice in the Description page of Project Settings.

#include "BreakableObject.h"
#include "Treasure.h"
#include "Components/CapsuleComponent.h"
#include "GeometryCollection/GeometryCollectionComponent.h"

// Sets default values
ABreakableObject::ABreakableObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	//BaseScene = CreateDefaultSubobject<USceneComponent>("BaseScene");
	//SetRootComponent(BaseScene);

	CapsuleCollision = CreateDefaultSubobject<UCapsuleComponent>("CapsuleCollision");
	SetRootComponent(CapsuleCollision);
	// CapsuleCollision->SetupAttachment(GetRootComponent());

	BreakabeObjectGeometryCollection = CreateDefaultSubobject<UGeometryCollectionComponent>(TEXT("GeometryCollection"));
	BreakabeObjectGeometryCollection->SetupAttachment(GetRootComponent());
	//SetRootComponent(BreakabeObjectGeometryCollection);
}

// Called when the game starts or when spawned
void ABreakableObject::BeginPlay()
{
	Super::BeginPlay();
}

void ABreakableObject::GetHit_Implementation(const FVector& HitLocation)
{
	// UE_LOG(LogTemp, Error, TEXT("Pot Got Hit!"))
	// FVector PotLocation = GetActorLocation();
	UE_LOG(LogTemp, Error, TEXT("%i"), bIsBroken)

	if (bIsBroken)
		return;
	UE_LOG(LogTemp, Error, TEXT( "Pot Hitted!!!"))

	FTransform SpawnTransform;
	SpawnTransform.SetLocation(FVector(GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z + 30.f));

	int8 RandomTreasure = FMath::RandRange(-1, Treasures.Num() - 1);

	if (RandomTreasure == -1)
		return;

	if (Treasures.Num() > 0)
		GetWorld()->SpawnActor<ATreasure>(Treasures[RandomTreasure], SpawnTransform);

	bIsBroken = true;





}

// Called every frame
void ABreakableObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// UE_LOG(LogTemp, Error, TEXT("TICKED!!!"));
}

