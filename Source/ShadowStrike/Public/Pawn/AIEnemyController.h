#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AIEnemyController.generated.h"

/**
 *
 */

class USpotLightComponent;

UCLASS()
class SHADOWSTRIKE_API AAIEnemyController : public AAIController
{
	GENERATED_BODY()
	
public:
	AEnemyPawn();

	
protected:
	virtual BeginPlay() override;
	void Patrol();

	UPROPERTY(EditAnywhere, Category = "AI Properties")
	TArray<FVector> ControlPoint;

	FVector* ActualTarget;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USpotLightComponent* SpotLightComponent;
};
