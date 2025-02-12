// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/BaseWeapon.h"
#include "AK.generated.h"

class ABullet;
/**
 * 
 */
UCLASS()
class SHADOWSTRIKE_API AAK : public ABaseWeapon
{
	GENERATED_BODY()
public:
	AAK();

protected:
	virtual void Fire() override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bullet", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<ABullet> Bullet;

	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bullet", meta = (AllowPrivateAccess = "true"))
	FVector BulletScale = FVector(5, 5, 5);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = "true"))
	USceneComponent* BulletSpawnLocation;

	
};
