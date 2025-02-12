#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyCharacter.h"
#include "AIEnemyController.generated.h"

/**
 *
 */

UCLASS()
class SHADOWSTRIKE_API AAIEnemyController : public AAIController
{
	GENERATED_BODY()
	

protected:
	virtual void BeginPlay() override;

	virtual void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result) override;

	void MoveToNextLocation();
	void SetNextLocation();

	AEnemyCharacter* ControlledCharacter;
	ATargetPoint* ActualTarget;

private:
	
};
