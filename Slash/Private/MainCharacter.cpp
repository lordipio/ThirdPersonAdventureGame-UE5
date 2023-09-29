// Fill out your copyright notice in the Description page of Project Settings.
#include "MainCharacter.h"
#include "Components/SkeletalMeshComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Weapon.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"
#include "GameFramework/GameModeBase.h"
#include "HealthOverlay.h"
#include "HUDs/OverlayHUD.h"
#include "Kismet/KismetMathLibrary.h"
#include "Item.h"


// Sets default values
AMainCharacter::AMainCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	Hair = CreateDefaultSubobject<USkeletalMeshComponent>("Hair");
	Hair->SetupAttachment(GetMesh());
	Eyebrow = CreateDefaultSubobject<USkeletalMeshComponent>("Eyebrow");
	Eyebrow->SetupAttachment(GetMesh());

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>("CameraBoom");
	CameraBoom->SetupAttachment(GetRootComponent());
	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetupAttachment(CameraBoom);

	Tags.Add(FName("Player"));
}

// Called when the game starts or when spawned
void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();
	AnimInstance = GetMesh()->GetAnimInstance();
	GameMode = GetWorld()->GetAuthGameMode<AGameModeBase>();

	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		AOverlayHUD* OverlayHUD = Cast<AOverlayHUD>(PlayerController->GetHUD());
		OverlayWidget = OverlayHUD->GetOverlayWidget();
	}

	if (OverlayWidget)
	{
		OverlayWidget->SetCoinNumber(0);
		OverlayWidget->SetSoulNumber(0);
		OverlayWidget->SetStaminaBar(0.5f);
	}
}

// Called every frame
void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	IncreaseStamina(0.03f, DeltaTime);
}

// Called to bind functionality to input
void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis(FName("MoveForward"), this, &AMainCharacter::MoveForward);
	PlayerInputComponent->BindAxis(FName("MoveRight"), this, &AMainCharacter::MoveRight);
	PlayerInputComponent->BindAxis(FName("Turn"), this, &AMainCharacter::Turn);
	PlayerInputComponent->BindAxis(FName("LookUp"), this, &AMainCharacter::LookUp);
	PlayerInputComponent->BindAction(FName("Equip"), EInputEvent::IE_Pressed, this, &AMainCharacter::Equip);
	PlayerInputComponent->BindAction(FName("Attack"), EInputEvent::IE_Pressed, this, &AMainCharacter::Attack);
	PlayerInputComponent->BindAction(FName("StopAttack"), EInputEvent::IE_Pressed, this, &AMainCharacter::StopAttack);
	PlayerInputComponent->BindAction(FName("Roll"), EInputEvent::IE_Pressed, this, &AMainCharacter::Roll);
}



void AMainCharacter::MoveForward(float Val)
{
	//if (Controller && Val != 0.f && CharacterMovementState == ECharacterMovementState::ECAS_Move)
	//	AddMovementInput(Camera->GetForwardVector(), Val);
	//UE_LOG(LogTemp, Error, TEXT("%s"), *(Camera->GetForwardVector().ToString()))
	
	if (Controller && (Val != 0.f) && CharacterMovementState == ECharacterMovementState::ECAS_Move)
	{
		const FRotator ControlRotation = Controller->GetControlRotation();
		const FRotator ControlRotationYaw(0.f, ControlRotation.Yaw, 0.f);
		const FVector Direction = FRotationMatrix(ControlRotationYaw).GetUnitAxis(EAxis::X);
		// const FVector Direction = ControlRotationYaw.Vector();
		// const FVector Direction = Camera->GetForwardVector();
		// FVector::DotProduct(FRotationMatrix(ControlRotationYaw), FVector(0.f, 1.f, 0.f));
		// UKismetMathLibrary::Vector4_DotProduct(FRotationMatrix(ControlRotationYaw).TransformFVector4(), FVector(0.f, 1.f, 0.f));
		// UE_LOG(LogTemp, Error, TEXT("%s"), *(Direction.ToString()))
		// UE_LOG(LogTemp, Error, TEXT("%s"), *(FRotationMatrix(ControlRotationYaw).ToString()))
		AddMovementInput(Direction, Val);
	}
}

void AMainCharacter::MoveRight(float Val)
{
	if (Controller && Val != 0.f && CharacterMovementState == ECharacterMovementState::ECAS_Move)
		AddMovementInput(Camera->GetRightVector(), Val);
}

void AMainCharacter::Turn(float Val)
{
	if (Controller)
		AddControllerYawInput(Val);
}

void AMainCharacter::LookUp(float Val)
{
	if (Controller)
		AddControllerPitchInput(-Val);
}

void AMainCharacter::Equip()
{
	AWeapon* Weapon = Cast<AWeapon>(CollidedItem);

	if (!Weapon && EquipmentState == ECharacterEquipmentState::ECS_EquippedItem && ArmState == ECharacterArmState::ECAS_Unarmed)
	{
		Arm();
	}

	else if (!Weapon && EquipmentState == ECharacterEquipmentState::ECS_EquippedItem && ArmState == ECharacterArmState::ECAS_Armed)
	{
		Disarm();
	}

	if (CollidedItem && Weapon)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ArmingSwordSound, GetActorLocation());
		Weapon->MeshRepresenter->AttachToComponent(this->GetMesh(), AttachmentRules, FName("LeftHandSocket"));
		EquipmentState = ECharacterEquipmentState::ECS_EquippedItem;
		Weapon->SphereComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		EquippedWeapon = Weapon;
		Weapon->SetItemState(true);
		ArmState = ECharacterArmState::ECAS_Armed;

	}
}

void AMainCharacter::Attack()
{
	if (AnimInstance && AttackState == ECharacterAttackState::ECAS_NoAttacking && ArmState == ECharacterArmState::ECAS_Armed && CharacterMovementState == ECharacterMovementState::ECAS_Move)
	{
		AnimInstance->Montage_Play(AttackMontage);
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

		AnimInstance->Montage_JumpToSection(MontageSection);
		AttackState = ECharacterAttackState::ECAS_Attacking;
		CharacterMovementState = ECharacterMovementState::ECAS_NoMovement;

	}
	else
		UE_LOG(LogTemp, Error, TEXT("Error!"))
}

FName AMainCharacter::GetHitDirection(const FVector& HitLocation)
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
		// HitDirection = ECharacterHitDirection::ECHD_Forward;
		DirectionName = FName("Front");
	}

	if (ThetaDegree >= 45.f && ThetaDegree <= 135.f)
	{
		// HitDirection = ECharacterHitDirection::ECHD_Left;
		DirectionName = FName("Left");
	}

	if (ThetaDegree <= -45.f && ThetaDegree > -135.f)
	{
		// HitDirection = ECharacterHitDirection::ECHD_Right;
		DirectionName = FName("Right");
	}

	if (ThetaDegree <= -135.f || ThetaDegree > 135.f)
	{
		// HitDirection = ECharacterHitDirection::ECHD_Backward;
		DirectionName = FName("Back");
	}

	//if (!EnemyAnimMontage)
	//	UE_LOG(LogTemp, Error, TEXT("Anim Montage is NULL"));

	return DirectionName;
}


void AMainCharacter::GetHit_Implementation(const FVector& HitLocation)
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


	if (AnimInstance && DamageAnimMontage)
	{
		AnimInstance->Montage_Play(DamageAnimMontage);
		AnimInstance->Montage_JumpToSection(MontageSectionName, DamageAnimMontage);
	}

}

void AMainCharacter::SpawnParticle(UWorld* MyWorld, UParticleSystem* HitParticle, FVector SpawnPoint)
{
}


float AMainCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	CurrentHealth = FMath::Clamp(CurrentHealth - Damage, 0.f, CurrentHealth);
	UE_LOG(LogTemp, Error, TEXT("%f"), CurrentHealth);


	if (OverlayWidget)
		OverlayWidget->SetHealthBar(CurrentHealth);

	if (CurrentHealth <= 0.f)
		HandleDeath();
	return 0.0f;
}


void AMainCharacter::StopAttack()
{
	AttackState = ECharacterAttackState::ECAS_NoAttacking;
}

void AMainCharacter::AttackEnd()
{
	UE_LOG(LogTemp, Error, TEXT("Attack Ends!"))
	AttackState = ECharacterAttackState::ECAS_NoAttacking;
	CharacterMovementState = ECharacterMovementState::ECAS_Move;
	RollState = ECharacterRollState::ECAS_NotRolling;

}


void AMainCharacter::Arm()
{
	if (CharacterMovementState == ECharacterMovementState::ECAS_NoMovement)
		return;
	AnimInstance->Montage_Play(ArmedMontage);
	AnimInstance->Montage_JumpToSection(FName("Arm"));

	ArmState = ECharacterArmState::ECAS_Armed;
	CharacterMovementState = ECharacterMovementState::ECAS_NoMovement;
}

void AMainCharacter::Disarm()
{
	if (CharacterMovementState == ECharacterMovementState::ECAS_NoMovement)
		return;
	ArmState = ECharacterArmState::ECAS_Unarmed;
	AnimInstance->Montage_Play(ArmedMontage);
	AnimInstance->Montage_JumpToSection(FName("Disarm"));

	CharacterMovementState = ECharacterMovementState::ECAS_NoMovement;

}

void AMainCharacter::Attach()
{
	EquippedWeapon->MeshRepresenter->AttachToComponent(this->GetMesh(), AttachmentRules, FName("WeaponHolder"));
	// EquipmentState = ECharacterEquipmentState::ECS_UneuippedItem;
}

void AMainCharacter::Detach()
{
	EquippedWeapon->MeshRepresenter->AttachToComponent(this->GetMesh(), AttachmentRules, FName("LeftHandSocket"));
	EquipmentState = ECharacterEquipmentState::ECS_EquippedItem;

}

void AMainCharacter::ArmEndAnim()
{
	CharacterMovementState = ECharacterMovementState::ECAS_Move;
}

void AMainCharacter::SetWeaponBoxCollision(ECollisionEnabled::Type CollisionType) const
{
	if (EquippedWeapon)
		EquippedWeapon->SetWeaponBoxCollision(CollisionType);
}


void AMainCharacter::DeathAnimationTimeEnded()
{
	AnimInstance->Montage_Pause(PlayerDeathMontage);
	
	// AnimInstance->StopAllMontages(0.f);
	

	UE_LOG(LogTemp, Error, TEXT("Death Ended!!!"))
}

void AMainCharacter::HandleDeath()
{
	Tags.Empty();
	if (ActorHasTag(FName("Player")))
		UE_LOG(LogTemp, Error, TEXT("There is Player Tag!"))

	CharacterMovementState = ECharacterMovementState::ECAS_NoMovement;

	FName DeathMontageSection = FName("DeathRight");
	if (HitDirectionName == FName("Left"))
		DeathMontageSection = FName("DeathLeft");

	if (HitDirectionName == FName("Right"))
		DeathMontageSection = FName("DeathRight");

	if (HitDirectionName == FName("Front"))
		DeathMontageSection = FName("DeathBackward");

	if (HitDirectionName == FName("Back"))
		DeathMontageSection = FName("DeathForward");

	AnimInstance->Montage_Play(PlayerDeathMontage);

	AnimInstance->Montage_JumpToSection(DeathMontageSection);
}

void AMainCharacter::Roll()
{
	if (Stamina <= 0.3f)
		return;

	if (RollState == ECharacterRollState::ECAS_NotRolling)
	{
		AnimInstance->Montage_Play(RollingMontage);
		AnimInstance->Montage_JumpToSection(FName("Default"));
		DecreaseStamina(0.3f);
		RollState = ECharacterRollState::ECAS_Rolling;
		AttackState = ECharacterAttackState::ECAS_NoAttacking;
		CharacterMovementState = ECharacterMovementState::ECAS_Move;
	}
}

void AMainCharacter::DecreaseStamina(float DecreaseValue)
{
	Stamina = FMath::Clamp(Stamina - DecreaseValue, 0.f, 1.f);
}

void AMainCharacter::IncreaseStamina(float IncreaseValue, float DeltaTime)
{
	Stamina = FMath::Clamp(Stamina + IncreaseValue * DeltaTime, 0.f, 1.f);
	if (OverlayWidget)
		OverlayWidget->SetStaminaBar(Stamina);
}

void AMainCharacter::RollEnded()
{
	RollState = ECharacterRollState::ECAS_NotRolling;
}


void AMainCharacter::GetHitAnimEnded()
{
	RollState = ECharacterRollState::ECAS_NotRolling;
	// EquipmentState = ECharacterEquipmentState::ECS_UneuippedItem;
	AttackState = ECharacterAttackState::ECAS_NoAttacking;
	CharacterMovementState = ECharacterMovementState::ECAS_Move;
}
