// Fill out your copyright notice in the Description page of Project Settings.
#include "Item.h"
#include "Components/SphereComponent.h"
#include "NiagaraComponent.h"
#include "HUDs/OverlayHUD.h"
#include "GameFramework/Character.h"
#include "HealthOverlay.h"
#include "GameAttributes.h"
#include "MainCharacter.h"

// Sets default values
AItem::AItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshRepresenter = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisualMesh"));
	SetRootComponent(MeshRepresenter);

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComponent->SetupAttachment(GetRootComponent());

	HoverParticle = CreateDefaultSubobject<UNiagaraComponent>(TEXT("HoverParticle"));
	HoverParticle->SetupAttachment(GetRootComponent());


}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();

	MyHUD = Cast<AOverlayHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
	if (MyHUD)
	{
		UE_LOG(LogTemp, Error, TEXT("HUD's Here!"))
		// MyWidget = MyHUD->GetOverlayWidget();
	}

	// SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AItem::Collided);
}


// Called every frame
void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsHover)
	{
		HoverActor(DeltaTime);
		RotateActor(DeltaTime);

	}



}

void AItem::EventEndOverlapFunction(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (CurrentMainCharacter)
	{
		CurrentMainCharacter->SetOverlappedItem(NULL);
		MainCharacter = CurrentMainCharacter;
	}
	CurrentMainCharacter = NULL;
	
}


void AItem::HoverActor(float DeltaTime)
{
	StopWatch += (DeltaTime * (DeltaTime * 300.f));
	if (StopWatch > 720.f)
		StopWatch = 0.f;
	SinMovementOffset = FVector(0.f, 0.f, FMath::Sin(StopWatch));
	// UE_LOG(LogTemp, Error, TEXT("%f"), SinMovementOffset.Z)
		AddActorLocalOffset(SinMovementOffset);

	RotateActor(DeltaTime);
}

void AItem::Collided(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Destroy();
;}

void AItem::EventBeginOverlapFunction(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	CurrentMainCharacter = Cast<AMainCharacter>(OtherActor);
	if (CurrentMainCharacter)
	{
		CurrentMainCharacter->SetOverlappedItem(this);
	}
}

void AItem::AddCoinsNumber(int Value)
{
	UGameAttributes* GameAttributes = Cast<UGameAttributes>(GetGameInstance());
	MyWidget = MyHUD->GetOverlayWidget();

	if (GameAttributes && MyWidget)
	{
		GameAttributes->SetCoinNumber(Value + GameAttributes->GetCoinNumber());
		MyWidget->SetCoinNumber(GameAttributes->GetCoinNumber());
	}
}


void AItem::AddSoulsNumber(int Value)
{
	UGameAttributes* GameAttributes = Cast<UGameAttributes>(GetGameInstance());
	MyWidget = MyHUD->GetOverlayWidget();

	if (GameAttributes && MyWidget)
	{
		GameAttributes->SetSoulNumber(Value + GameAttributes->GetSoulNumber());
		MyWidget->SetSoulNumber(GameAttributes->GetSoulNumber());
	}
}


void AItem::DestroyItem()
{
	Destroy();
}