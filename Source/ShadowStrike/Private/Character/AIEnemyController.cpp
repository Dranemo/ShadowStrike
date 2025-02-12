#include "Character/AIEnemyController.h"



void AAIEnemyController::BeginPlay()
{
	Super::BeginPlay();

	ControlledCharacter = Cast<AEnemyCharacter>(GetPawn());
	if (ControlledCharacter && ControlledCharacter->ControlPoint.Num() > 0)
	{
		ActualTarget = ControlledCharacter->ControlPoint[0];
		MoveToNextLocation();
	}
}


void AAIEnemyController::MoveToNextLocation()
{
	if (ControlledCharacter)
	{
		MoveToLocation(ActualTarget->GetActorLocation());
	}
}

void AAIEnemyController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	Super::OnMoveCompleted(RequestID, Result);
	
	SetNextLocation();
	
	MoveToNextLocation();
}

void AAIEnemyController::SetNextLocation()
{
	if(ControlledCharacter->ControlPoint.IndexOfByKey(ActualTarget) + 1 < ControlledCharacter->ControlPoint.Num())
	{
		ActualTarget = ControlledCharacter->ControlPoint[ControlledCharacter->ControlPoint.IndexOfByKey(ActualTarget) + 1];
	}
	else
	{
		ActualTarget = ControlledCharacter->ControlPoint[0];
	}
}
