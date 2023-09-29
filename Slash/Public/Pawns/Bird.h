// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Bird.generated.h"

class UFloatingPawnMovement;
class UCapsuleComponent;
class USkeletalMeshComponent;
class USpringArmComponent;
class UCameraComponent;
UCLASS()
class SLASH_API ABird : public APawn
{
	GENERATED_BODY()

public:
	ABird();
	virtual void Tick(float) override;
	virtual void SetupPlayerInputComponent(UInputComponent*) override;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere)
	UCapsuleComponent* BirdCapsuleComponent;
	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* BirdSkeletalMeshComponent;
	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* CameraBoomComponent;
	UPROPERTY(VisibleAnywhere)
	UCameraComponent* CameraComponent;
	UPROPERTY(VisibleAnywhere)
	UFloatingPawnMovement* FloatingMovementComponent;

	void MoveForward(float);
	void Turn(float Value);
	void LookUp(float Value);
};
