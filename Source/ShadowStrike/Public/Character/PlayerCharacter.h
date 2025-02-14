// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "Character/BaseCharacter.h"
#include "PlayerCharacter.generated.h"


class ABaseWeapon;
class UCameraComponent;
class USpringArmComponent;
class UInputAction;
class UCapsuleComponent;
class UInputMappingContext;

/**
 * 
 */
UCLASS()
class SHADOWSTRIKE_API APlayerCharacter : public ABaseCharacter
{
	GENERATED_BODY()

public:
	APlayerCharacter();

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	bool IsHidden = false;

	virtual void Die() override;

	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	virtual void Tick(float DeltaSeconds) override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UCapsuleComponent* CapsuleCollision;

	APlayerController* PlayerController;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Anim", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* DeathAnimMontage;

	FTimerHandle DeathHandle;
	void RespawnDeath();


protected:
	virtual void BeginPlay() override;
	virtual void Move(const FInputActionValue& Value);
	virtual void Fire() override;
	void Hide();
	void Pickup();

	UPROPERTY(EditAnywhere, Category = "Controller")
	UInputMappingContext* PlayerMappingContext;

	UPROPERTY(EditAnywhere, Category = "Controller")
	UInputAction* MovementAction;

	UPROPERTY(EditAnywhere, Category = "Controller")
	UInputAction* FireAction;

	UPROPERTY(EditAnywhere, Category = "Controller")
	UInputAction* HideAction;

	UPROPERTY(EditAnywhere, Category = "Controller")
	UInputAction* PickUpAction;

	UPROPERTY(EditAnywhere, Category = "Sound")
	USoundBase* HideSound;
};
