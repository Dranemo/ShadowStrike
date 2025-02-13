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
	virtual void Tick(float DeltaSeconds) override;

	virtual void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result) override;

	void MoveToNextLocation();
	void SetNextLocation();
	void CheckRotationFinished(FVector Direction, FRotator TargetRotation);

	AEnemyCharacter* ControlledCharacter;
	ATargetPoint* ActualTarget;
	FTimerHandle RotationTimerHandle;

	bool TurnAnimStarted = false;
	bool MovementStopped = false;
	
private:
	
};
