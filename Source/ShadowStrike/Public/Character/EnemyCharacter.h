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

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category = "AI Properties")
	TArray<ATargetPoint*> ControlPoint;

	UPROPERTY(EditAnywhere, Category="Animation")
	UAnimMontage* LeftTurnAnimMontage;

protected:
	virtual void BeginPlay() override;

	bool CheckPlayerDetection();

	APlayerCharacter* PlayerPawnCharacter;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USpotLightComponent* SpotLightComponent;
};
