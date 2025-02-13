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

	UPROPERTY(EditAnywhere, Category = "AI Properties")
	TArray<ATargetPoint*> ControlPoint;

	UPROPERTY(EditAnywhere, Category="Animation")
	UAnimMontage* LeftTurnAnimMontage;

	UPROPERTY(EditAnywhere, Category="Animation")
	UAnimMontage* RightTurnAnimMontage;
	
	UPROPERTY(EditAnywhere, Category="Animation")
	UAnimMontage* DownToAimAnimMontage;

	UPROPERTY(EditAnywhere, Category="Animation")
	UAnimMontage* AimToDownAnimMontage;

	UPROPERTY(EditAnywhere, Category="Animation")
	UAnimMontage* FiringAnimMontage;

protected:
	virtual void BeginPlay() override;
	virtual void Fire() override;
	

	bool CheckPlayerDetection();
	void WaitDetectionDelay();

	APlayerCharacter* PlayerPawnCharacter;
	FTimerHandle FiringCooldownHandle;


private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USpotLightComponent* SpotLightComponent;

	FTimerHandle DetectionDelayTimerHandle;
	bool PlayerDetected = false;
};
