#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyPawn.h"
#include "AIEnemyController.generated.h"

/**
 *
 */

class USpotLightComponent;

UCLASS()
class SHADOWSTRIKE_API AAIEnemyController : public AAIController
{
	GENERATED_BODY()
	

protected:
	virtual void BeginPlay() override;
	void Patrol();

	AEnemyPawn* ControlledPawn;
	FVector ActualTarget;

private:
	
};
