#include "Pawn/AIEnemyController.h"

#include "Pawn/BasePawn.h"
#include "Pawn/EnemyPawn.h"

void AAIEnemyController::BeginPlay()
{
	Super::BeginPlay();

	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("Test0"));
	ControlledPawn = Cast<AEnemyPawn>(GetPawn());
	ActualTarget = ControlledPawn->ControlPoint[0];
	Patrol();
}

void AAIEnemyController::Patrol()
{
	if (ControlledPawn)
	{
		MoveToLocation(ActualTarget);
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("IN"));
	}
}