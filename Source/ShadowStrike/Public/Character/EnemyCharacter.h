#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "PlayerCharacter.h"
#include "Components/SpotLightComponent.h"
#include "Engine/TargetPoint.h"
#include "EnemyCharacter.generated.h"

UCLASS()
class SHADOWSTRIKE_API AEnemyCharacter : public ABaseCharacter
{
	GENERATED_BODY()

public:
	AEnemyCharacter();

	virtual void Die() override;

	virtual void Tick(float DeltaTime) override;

	bool GetPlayerDetected() const { return PlayerDetected; }

	void DisbaleAllTimer();

	APlayerCharacter* PlayerPawnCharacter;

	UPROPERTY(EditAnywhere, Category = "AI Properties")
	TArray<ATargetPoint*> ControlPoint;

	UPROPERTY(EditAnywhere, Category="Animation")
	UAnimMontage* LeftTurnAnimMontage;

	UPROPERTY(EditAnywhere, Category="Animation")
	UAnimMontage* RightTurnAnimMontage;

protected:
	virtual void BeginPlay() override;
	virtual void Fire() override;
	

	bool CheckPlayerDetection();
	void WaitDetectionDelay();
	void SetRifleFiringTransform(FVector TargetLocation, FRotator TargetRotation);
	void LookAtPlayer();
	
	
	FTimerHandle FiringCooldownHandle;
	FTimerHandle RifleTransformTimerHandle;
	FTimerHandle LookAtPlayerHandle;

	UPROPERTY(EditAnywhere, Category = "Components")
	USceneComponent* FiringWeaponLocation;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USpotLightComponent* SpotLightComponent;

	FTimerHandle DetectionDelayTimerHandle;
	bool PlayerDetected = false;

	UPROPERTY(EditAnywhere, Category = "Sound")
	USoundBase* GuardDetectedPlayerSound;
};
