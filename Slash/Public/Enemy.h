// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "IHitInterface.h"
#include "CharacterState.h"
#include "Enemy.generated.h"

class UCharacterAttribute;
class UHealthBarWidgetComponent;
class AAIController;
class UPawnSensingComponent;
class AWeapon;

UCLASS()
class SLASH_API AEnemy : public ACharacter, public IIHitInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy();

private:
	bool bDrawDebugDirection = false;
	FTimerHandle HealthBarTimer;
	void SetHealthBarInvisible();

	UPROPERTY(EditInstanceOnly, meta = (AllowPrivateAccess = "true"), Category = "AI")
		TArray<AActor*> TargetPoints;


	AAIController* EnemyAIController = NULL;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UAnimInstance* EnemyAnimInstance = NULL;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		UCharacterAttribute* CharacterAttributeComponent;

	UPROPERTY(VisibleAnywhere)
		UHealthBarWidgetComponent* HealthBarWidgetComponent = NULL;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void GetHit_Implementation(const FVector& HitLocation);

	UPROPERTY(EditDefaultsOnly)
		UAnimMontage* EnemyAnimMontage;

	UPROPERTY(EditDefaultsOnly)
		UPawnSensingComponent* EnemySensingComponent = NULL;

	UPROPERTY(EditDefaultsOnly)
		USoundBase* HitSound = NULL;

	UPROPERTY(EditDefaultsOnly)
		UParticleSystem* TestParticle = NULL;

	UPROPERTY(EditDefaultsOnly)
		UAnimMontage* EnemyDeathMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float CurrentHealth = 1.f;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser);

	UFUNCTION(BlueprintCallable)
		void DeathAnimationTimeEnded();


	ECharacterHitDirection HitDirection = ECharacterHitDirection::ECHD_Forward;

	void HandleDeath();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void HandleDeathAnimEvent();

	bool CheckIfObjectIsInRange(float Treshold, AActor* OtherActor);

	APawn* PlayerPawn = NULL;

	void AIMoveTo(AActor* GoalActor, float Radius);

	void HandleMovement();

	EEnemyState EnemyState = EEnemyState::EES_Patrol;

	AActor* SelectRandomActor(TArray<AActor*> Actors);

	AActor* CurrentPatrolTarget = NULL;

	FTimerHandle PatrolTimer;

	void PatrolWait();

	AActor* MoveGoalActor = NULL;

	UFUNCTION()
		void OnSeePawn(APawn* OtherPawn);

	bool bEnemySeePlayer = false;

	ECharacterHealth EnemyHealthState = ECharacterHealth::ECH_Alive;

	void Attack();

	void StopAttack();

	UFUNCTION(BlueprintCallable)
		void AttackEnd();


	UPROPERTY(EditDefaultsOnly)
		UAnimMontage* EnemyAttackMontage = NULL;

	UPROPERTY(EditDefaultsOnly)
		float CombatRange = 250.f;

	UAnimInstance* EnemyAnim = NULL;

	ECharacterAttackState EnemyAttackState = ECharacterAttackState::ECAS_NoAttacking;

	UPROPERTY(EditAnywhere)
		TSubclassOf<AWeapon> WeaponClass = NULL;

	UPROPERTY(EditAnywhere)
		UObject* WeaponObject = NULL;

	UPROPERTY(BlueprintReadOnly)
		AWeapon* EnemyWeapon = NULL;

	UFUNCTION(BlueprintCallable)
		void SetWeaponBoxCollision(ECollisionEnabled::Type CollisionType) const;

	 UFUNCTION(BlueprintCallable)
		 FVector GetWarpRotation();

	 UFUNCTION(BlueprintCallable)
		FVector GetWarpTranslation();


	FName GetHitDirection(const FVector& HitLocation);
	FName HitDirectionName = FName("");

	UPROPERTY(EditAnywhere)
	float Damage = 0.3f;

	UPROPERTY(EditDefaultsOnly)
	class UNiagaraSystem* SoulParticle;

	void SpawnSoul();

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class ASoul> SoulClass;

	UPROPERTY(EditDefaultsOnly)
	ECharacterAttackState Test = ECharacterAttackState::ECAS_Attacking;

	UFUNCTION(BlueprintCallable)
	void GetHitAnimEnd();
};