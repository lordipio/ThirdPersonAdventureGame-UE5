// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy.h"
#include "Components/CharacterAttribute.h"
#include "Components/HealthBarWidgetComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/PawnSensingComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Weapon.h"
#include "Components/SphereComponent.h"
#include "Soul.h"
#include "Niagara/Public/NiagaraFunctionLibrary.h"
#include "AIController.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	CharacterAttributeComponent = CreateDefaultSubobject<UCharacterAttribute>(TEXT("CharacterAttributes"));
	HealthBarWidgetComponent = CreateDefaultSubobject<UHealthBarWidgetComponent>(TEXT("HealthBarWidgetComponent1"));
	HealthBarWidgetComponent->SetupAttachment(GetRootComponent());
	EnemySensingComponent = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("EnemySensingComponent"));
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();


	EnemyAnim = GetMesh()->GetAnimInstance();
	if (!EnemyAnim)
		UE_LOG(LogTemp, Error, TEXT("There is no Enemy Anim Instance!"))

	EnemySensingComponent->OnSeePawn.AddDynamic(this, &AEnemy::OnSeePawn);

	CurrentPatrolTarget = SelectRandomActor(TargetPoints);
	MoveGoalActor = CurrentPatrolTarget;

	PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);
	if (!PlayerPawn)
		UE_LOG(LogTemp, Error, TEXT("PlayerPawn is NULL!"))

	if (CurrentHealth)
		CurrentHealth = CharacterAttributeComponent->HealthPercentage;

	HealthBarWidgetComponent->SetHealthProgressBar(CurrentHealth);
	EnemyAIController = Cast<AAIController>(GetController());

	if (!EnemyAIController)
		UE_LOG(LogTemp, Error, TEXT("Can't Cast To AI!"))

	
	FAttachmentTransformRules WeaponAttachmentRules = FAttachmentTransformRules::KeepRelativeTransform;

	if (WeaponClass)
	{
		AActor* SpawnedActor = GetWorld()->SpawnActor(WeaponClass);
		if (SpawnedActor)
			EnemyWeapon = EnemyWeapon = Cast<AWeapon>(SpawnedActor);
		if (EnemyWeapon)
			EnemyWeapon->MeshRepresenter->AttachToComponent(GetMesh(), WeaponAttachmentRules, FName("RightHandSocket"));
		else UE_LOG(LogTemp, Error, TEXT("Enemy Weapon is NOT Okay!"))
	}
	else UE_LOG(LogTemp, Error, TEXT("Enemy Weapon is NOT Okay!"))

	EnemyWeapon->SphereComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	EnemyWeapon->SetItemState(true);
	EnemyWeapon->SetOwner(this);

}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	

	HealthBarWidgetComponent->SetHiddenInGame(!CheckIfObjectIsInRange(1000.f, PlayerPawn));

	HandleMovement();

}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemy::GetHit_Implementation(const FVector& HitLocation)
{
	HitDirectionName = GetHitDirection(HitLocation);
	FName MontageSectionName = FName("");

	if (HitDirectionName == FName("Front"))
		MontageSectionName = FName("StartImpactFromFront");
	if (HitDirectionName == FName("Back"))
		MontageSectionName = FName("StartImpactFromBack");
	if (HitDirectionName == FName("Left"))
		MontageSectionName = FName("StartImpactFromLeft");
	if (HitDirectionName == FName("Right"))
		MontageSectionName = FName("StartImpactFromRight");


	EnemyAnimInstance = GetMesh()->GetAnimInstance();
	if (EnemyAnimInstance)
	{
		EnemyAnimInstance->Montage_Play(EnemyAnimMontage);
		EnemyAnimInstance->Montage_JumpToSection(MontageSectionName, EnemyAnimMontage);
		// UE_LOG(LogTemp, Error, TEXT("%s"), *(MontageSectionName.ToString()));
	}
	else
		UE_LOG(LogTemp, Error, TEXT("Anim Instance is NULL"));

	if (HitSound)
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), HitSound, HitLocation);

	USceneComponent* Scene = Cast<USceneComponent>(GetRootComponent());

	if (Scene)
		SpawnParticle(GetWorld(), TestParticle, HitLocation);

}

FName AEnemy::GetHitDirection(const FVector& HitLocation)
{
	FVector ActorForward = GetActorForwardVector();
	FVector ActorLocation = GetActorLocation();
	FVector ActorToHitLocation = HitLocation - ActorLocation;
	ActorToHitLocation = ActorToHitLocation.GetSafeNormal();


	float CosTheta = FVector::DotProduct(ActorForward, ActorToHitLocation);
	float ThetaRadian = FMath::Acos(CosTheta);
	float ThetaDegree = FMath::RadiansToDegrees(ThetaRadian);

	FVector CrossProduct = FVector::CrossProduct(ActorToHitLocation, ActorForward);
	if (CrossProduct.Z < 0)
		ThetaDegree = -ThetaDegree;


	FName DirectionName = FName("");


	if (ThetaDegree >= -45.f && ThetaDegree < 45.f)
	{
		HitDirection = ECharacterHitDirection::ECHD_Forward;
		DirectionName = FName("Front");
	}

	if (ThetaDegree >= 45.f && ThetaDegree <= 135.f)
	{
		HitDirection = ECharacterHitDirection::ECHD_Left;
		DirectionName = FName("Left");
	}

	if (ThetaDegree <= -45.f && ThetaDegree > -135.f)
	{
		HitDirection = ECharacterHitDirection::ECHD_Right;
		DirectionName = FName("Right");
	}

	if (ThetaDegree <= -135.f || ThetaDegree > 135.f)
	{
		HitDirection = ECharacterHitDirection::ECHD_Backward;
		DirectionName = FName("Back");
	}

	if (!EnemyAnimMontage)
		UE_LOG(LogTemp, Error, TEXT("Anim Montage is NULL"));
	

	return DirectionName;
}

float AEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	CurrentHealth =  FMath::Clamp(CurrentHealth - Damage, 0.f, CurrentHealth);

	EnemyState = EEnemyState::EES_Chase;


	HealthBarWidgetComponent->SetHealthProgressBar(CurrentHealth);

	if (CurrentHealth <= 0.f)
	{
		HandleDeath();
	}
	return 0.0f;
}

void AEnemy::DeathAnimationTimeEnded()
{
	UE_LOG(LogTemp, Error, TEXT("DeathAnimationTimeEnded"));
	EnemyAnimInstance->Montage_Pause(EnemyDeathMontage);
	
}

void AEnemy::HandleDeath()
{
	HandleDeathAnimEvent();
	// EnemyWeapon->SetActorEnableCollision(false);
	EnemyWeapon->SetWeaponBoxCollision(ECollisionEnabled::NoCollision);
	// SpawnParticle(GetWorld(), SoulParticle, GetActorLocation());
	SpawnSoul();
	EnemyHealthState = ECharacterHealth::ECH_Died;

	GetCharacterMovement()->bOrientRotationToMovement = false;

	FName DeathMontageSection = FName("DeathRight");
	if (HitDirectionName == FName("Left"))
		DeathMontageSection = FName("DeathLeft");

	if (HitDirectionName == FName("Right"))
		DeathMontageSection = FName("DeathRight");

	if (HitDirectionName == FName("Front"))
		DeathMontageSection = FName("DeathBackward");

	if (HitDirectionName == FName("Back"))
		DeathMontageSection = FName("DeathForward");

	EnemyAnimInstance->Montage_Play(EnemyDeathMontage);

	EnemyAnimInstance->Montage_JumpToSection(DeathMontageSection);

	GetWorldTimerManager().SetTimer(HealthBarTimer, this, &AEnemy::SetHealthBarInvisible, 1.f);

	
}

/***
returns True if Enemy is too far
***/
bool AEnemy::CheckIfObjectIsInRange(float Treshold, AActor* OtherActor)
{
	FVector EnemyLocation = GetActorLocation();
	if (OtherActor)
	{
		FVector OtherActorLocation = OtherActor->GetActorLocation();
		float Dist = (OtherActorLocation - EnemyLocation).Size();

		if (Dist >= Treshold) // far
			return false;

		else // close
			return true;
	}

	return false;
}

void AEnemy::SetHealthBarInvisible()
{
	HealthBarWidgetComponent->SetVisibility(false);
}

void AEnemy::HandleMovement()
{
	if (EnemyHealthState == ECharacterHealth::ECH_Died)
		return;

	if (EnemyState == EEnemyState::EES_Chase)
	{
		MoveGoalActor = PlayerPawn;
		GetCharacterMovement()->MaxWalkSpeed = 700.f;
	}

	if (!MoveGoalActor) 
		return;

	if (!CheckIfObjectIsInRange(1000.f, PlayerPawn))
	{
		EnemyState = EEnemyState::EES_Patrol;
		GetCharacterMovement()->MaxWalkSpeed = 100.f;
	}

	if (EnemyState == EEnemyState::EES_Patrol)
	{
		MoveGoalActor = CurrentPatrolTarget;
		if (CheckIfObjectIsInRange(300.f, MoveGoalActor)) // reach patrol point
		{
			MoveGoalActor = NULL;
			GetWorldTimerManager().SetTimer(PatrolTimer, this, &AEnemy::PatrolWait, FMath::RandRange(10.f, 10.f), false);
		}
	}

	if (CheckIfObjectIsInRange(CombatRange, PlayerPawn) && EnemyState != EEnemyState::EES_Patrol)
	{
		Attack();
	}

	AIMoveTo(MoveGoalActor, 100.f);
}

AActor* AEnemy::SelectRandomActor(TArray<AActor*> Actors)
{
	TArray<AActor*> NewActors;
	for (AActor* Actor : Actors)
	{
		if (Actor == CurrentPatrolTarget)
			continue;
		NewActors.Add(Actor);
	}

	return NewActors[FMath::RandRange(0, NewActors.Num() - 1)];
}

void AEnemy::AIMoveTo(AActor* GoalActor, float Radius)
{
	if (!GoalActor)
	{
		UE_LOG(LogTemp, Error, TEXT("GoalActor is NULL!"))
			return;
	}

	if (!EnemyAIController)
	{
		UE_LOG(LogTemp, Error, TEXT("EnemyAIController is NULL!"))
			return;
	}

	FAIMoveRequest EnemyAIMoveRequest;
	FNavPathSharedPtr AIPath;
	FPathFollowingRequestResult AIPathResult;

	EnemyAIMoveRequest.SetGoalActor(GoalActor);
	EnemyAIMoveRequest.SetAcceptanceRadius(Radius);

	AIPathResult = EnemyAIController->MoveTo(EnemyAIMoveRequest, &AIPath);
}

void AEnemy::PatrolWait()
{
	UE_LOG(LogTemp, Warning, TEXT("Change Patrol Target"));
	CurrentPatrolTarget = SelectRandomActor(TargetPoints);
	MoveGoalActor = CurrentPatrolTarget;
}

void AEnemy::OnSeePawn(APawn* OtherPawn)
{
	if (OtherPawn->ActorHasTag("Player") && CheckIfObjectIsInRange(1000.f, PlayerPawn))
	{
		EnemyState = EEnemyState::EES_Chase;
	}
	else
	{
		EnemyAttackState = ECharacterAttackState::ECAS_NoAttacking;
		EnemyState = EEnemyState::EES_Patrol;

	}
}



void AEnemy::Attack()
{
	if (EnemyAnim && EnemyAttackState == ECharacterAttackState::ECAS_NoAttacking)
	{
		UE_LOG(LogTemp, Error, TEXT("Attack!"))

		EnemyAnim->Montage_Play(EnemyAttackMontage);
		int32 RandNum = FMath::RandRange(3, 5);
		FName MontageSection;
		switch (RandNum)
		{
		case 3:
			MontageSection = FName("Attack3");
			break;
		case 4:
			MontageSection = FName("Attack4");
			break;
		case 5:
			MontageSection = FName("Attack5");
			break;
		default:
			break;
		}

		EnemyAnim->Montage_JumpToSection(MontageSection);
		EnemyAttackState = ECharacterAttackState::ECAS_Attacking;
	}

}

void AEnemy::StopAttack()
{
	EnemyAttackState = ECharacterAttackState::ECAS_NoAttacking;
}

void AEnemy::AttackEnd()
{
	StopAttack();
}

void AEnemy::SetWeaponBoxCollision(ECollisionEnabled::Type CollisionType) const
{	
	if (EnemyWeapon)
		EnemyWeapon->SetWeaponBoxCollision(CollisionType);
}


FVector AEnemy::GetWarpRotation()
{
	FVector WarpTarget = PlayerPawn->GetActorLocation();
	return WarpTarget;
}


FVector AEnemy::GetWarpTranslation()
{
	if (!PlayerPawn)
		return FVector::Zero();

	const FVector EnemyLocation = GetActorLocation();

	const FVector PlayerLocation = PlayerPawn->GetActorLocation();

	const FVector WarpTarget = PlayerLocation + ( (- PlayerLocation + EnemyLocation).GetSafeNormal() * 150.f );

	// DrawDebugDirectionalArrow(GetWorld(), EnemyLocation, WarpTarget, 10.f, FColor::Black, false, 4.f);

	return WarpTarget;
}

void AEnemy::SpawnSoul()
{
	// UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, SoulParticle, GetActorLocation());
	GetWorld()->SpawnActor<ASoul>(SoulClass, GetActorLocation(), FRotator::ZeroRotator);
}

void AEnemy::GetHitAnimEnd()
{
	UE_LOG(LogTemp, Error, TEXT("Enemy Get Hit Anim Ended2"))
	StopAttack();
}

