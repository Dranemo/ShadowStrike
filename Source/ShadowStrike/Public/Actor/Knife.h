// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/BaseWeapon.h"
#include "Knife.generated.h"

class UBoxComponent;

/**
 * 
 */
UCLASS()
class SHADOWSTRIKE_API AKnife : public ABaseWeapon
{
	GENERATED_BODY()

public:
	AKnife();

protected:
	virtual void Fire() override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UBoxComponent* TriggerBox;
};
