// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CharacterState.h"
#include "IHitInterface.h"
#include "MainCharacter.generated.h"

class AItem;
class UCameraComponent;
class USpringArmComponent;
class USkeletalMeshComponent;
class AWeapon;
class USoundBase;

UCLASS()
class SLASH_API AMainCharacter : public ACharacter, public IIHitInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMainCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void MoveForward(float Val);
	void MoveRight(float Val);
	void Turn(float Val);
	void LookUp(float Val);
	void Attack();
	void Equip();
	void StopAttack();

	UFUNCTION(BlueprintCallable)
	void AttackEnd();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* Hair;
	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* Eyebrow;
	UPROPERTY(VisibleAnywhere)
	UCameraComponent* Camera;
	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* CameraBoom;

	FORCEINLINE void SetOverlappedItem(AItem* OverlappedItem) { this->CollidedItem = OverlappedItem; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE  ECharacterEquipmentState GetCharacterState() const { return EquipmentState; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE  ECharacterAttackState GetCharacterAttackState() const { return AttackState; }

	//UFUNCTION
	FORCEINLINE ECharacterArmState GetCharacterArmState() const { return ArmState; }
	
	UFUNCTION(BlueprintCallable)
	FORCEINLINE ECharacterMovementState GetCharacterMovementState() const { return CharacterMovementState; }
	
	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* AttackMontage;

	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* ArmedMontage;

	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* RollingMontage;

	void Roll();

	UFUNCTION(BlueprintCallable)
	void Arm();

	UFUNCTION(BlueprintCallable)
	void Disarm();

	UFUNCTION(BlueprintCallable)
	void Attach();

	UFUNCTION(BlueprintCallable)
	void Detach();

	UFUNCTION(BlueprintCallable)
	void ArmEndAnim();

	UPROPERTY(EditAnywhere, Category = "Sounds")
	USoundBase* ArmingSwordSound = NULL;

private:
	FAttachmentTransformRules AttachmentRules = FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, false);;
	ECharacterEquipmentState EquipmentState = ECharacterEquipmentState::ECS_UneuippedItem;
	ECharacterAttackState AttackState = ECharacterAttackState::ECAS_NoAttacking;
	ECharacterArmState ArmState = ECharacterArmState::ECAS_Unarmed;

	ECharacterMovementState CharacterMovementState = ECharacterMovementState::ECAS_Move;

	AItem* CollidedItem;
	//void Unequip();
	UAnimInstance* AnimInstance = NULL;

	AWeapon* EquippedWeapon = NULL;

	UFUNCTION(BlueprintCallable)
	void SetWeaponBoxCollision(ECollisionEnabled::Type CollisionType) const;

	FName GetHitDirection(const FVector& HitLocation);

	virtual void GetHit_Implementation(const FVector& HitLocation);

	FName HitDirectionName;

	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* DamageAnimMontage;

	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* PlayerDeathMontage;
	

	virtual void SpawnParticle(UWorld* MyWorld, UParticleSystem* HitParticle, FVector SpawnPoint) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser);

	UPROPERTY(EditDefaultsOnly)
	float Damage = 0.5f;

	UPROPERTY(EditDefaultsOnly)
	float CurrentHealth = 1.f;

	AGameModeBase* GameMode;

	// APlayerController* PlayerController;
	class UHealthOverlay* OverlayWidget;

	UFUNCTION()
	void HandleDeath();

	UFUNCTION(BlueprintCallable)
		void DeathAnimationTimeEnded();

	void DecreaseStamina(float DecreaseValue);
	void IncreaseStamina(float IncreaseValue, float DeltaTime);

	float Stamina = 1.f;

	UFUNCTION(BlueprintCallable)
		void RollEnded();
	
	ECharacterRollState RollState = ECharacterRollState::ECAS_NotRolling;

	UFUNCTION(BlueprintCallable)
		void GetHitAnimEnded();
};
