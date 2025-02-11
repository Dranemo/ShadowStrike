#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
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

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USpotLightComponent* SpotLightComponent;
};
