// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "Pawn/BasePawn.h"
#include "PlayerPawn.generated.h"


class UCameraComponent;
class USpringArmComponent;
class UInputAction;
class USphereComponent;
class UInputMappingContext;

/**
 * 
 */
UCLASS()
class SHADOWSTRIKE_API APlayerPawn : public ABasePawn
{
	GENERATED_BODY()

public:
	APlayerPawn();
	bool IsHidden = false;

	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USphereComponent* SphereCollision;

	APlayerController* PlayerController;
	void HidingBindActions(bool _bool, UEnhancedInputComponent* EnhancedInputComponent);


protected:
	virtual void BeginPlay() override;
	virtual void Move(const FInputActionValue& Value);
	virtual void Fire() override;
	virtual void Hide();

	UPROPERTY(EditAnywhere, Category = "Controller")
	UInputMappingContext* PlayerMappingContext;

	UPROPERTY(EditAnywhere, Category = "Controller")
	UInputAction* MovementAction;

	UPROPERTY(EditAnywhere, Category = "Controller")
	UInputAction* FireAction;

	UPROPERTY(EditAnywhere, Category = "Controller")
	UInputAction* HideAction;
};
