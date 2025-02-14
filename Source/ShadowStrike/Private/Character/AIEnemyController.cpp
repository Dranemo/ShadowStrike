#include "Character/AIEnemyController.h"

#include "InteractiveToolManager.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Navigation/PathFollowingComponent.h"


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

void AAIEnemyController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (ControlledCharacter->GetIsDead())
		return;

	
	if (ControlledCharacter->GetPlayerDetected())
	{
		StopMovement();
		MovementStopped = true;
	}
	else if (MovementStopped)
	{
		MoveToNextLocation();
		MovementStopped = false;
	}
}


void AAIEnemyController::MoveToNextLocation()
{
	if (ControlledCharacter)
	{
		
		if (ControlledCharacter->GetIsDead())
		{
			StopMovement();
			return;
		}
		
		MoveToLocation(ActualTarget->GetActorLocation());
	}
}

void AAIEnemyController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)

{
	Super::OnMoveCompleted(RequestID, Result);

	
	if (ControlledCharacter->GetIsDead())
		return;

	if (Result.Flags == FPathFollowingResultFlags::MovementStop && Result.Flags == FPathFollowingResultFlags::NewRequest)
	{
		return;
	}
	
	SetNextLocation();

	FVector Direction = (ActualTarget->GetActorLocation() - ControlledCharacter->GetActorLocation()).GetSafeNormal();
	FRotator TargetRotation = FRotationMatrix::MakeFromX(-Direction).Rotator();
	
	GetWorldTimerManager().SetTimer(RotationTimerHandle, FTimerDelegate::CreateLambda([this, Direction, TargetRotation]()
	{
		CheckRotationFinished(-Direction, TargetRotation);
	}),0.01f, true);
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

void AAIEnemyController::CheckRotationFinished(FVector Direction, FRotator TargetRotation)
{
	if (!ControlledCharacter) return;
	
	if (ControlledCharacter->GetIsDead())
		return;

	if (ControlledCharacter->GetPlayerDetected())
	{
		GetWorld()->GetTimerManager().ClearTimer(RotationTimerHandle);
	}
	
	if(!TurnAnimStarted && ControlledCharacter->LeftTurnAnimMontage)
	{
		if (TargetRotation.Yaw > 0)
		{
			ControlledCharacter->PlayAnimMontage(ControlledCharacter->RightTurnAnimMontage);
		}
		else
		{
			ControlledCharacter->PlayAnimMontage(ControlledCharacter->LeftTurnAnimMontage);
		}
		
		TurnAnimStarted = true;
	}
	ControlledCharacter->SetActorRotation(FMath::RInterpTo(
		ControlledCharacter->GetActorRotation(), 
		 FRotator(0,TargetRotation.Yaw,0),                   
		 UGameplayStatics::GetWorldDeltaSeconds(this), 
		1.0f
	));
	
	FVector ForwardVector = ControlledCharacter->GetActorForwardVector();
	float DotProduct = FVector::DotProduct(ForwardVector, Direction);
	
	if (DotProduct > 0.99f ) 
	{
		TurnAnimStarted = false;
		GetWorld()->GetTimerManager().ClearTimer(RotationTimerHandle);
		if(Direction == (ActualTarget->GetActorLocation() - ControlledCharacter->GetActorLocation()).GetSafeNormal())
		{
			MoveToNextLocation(); 
		}
		else if (Direction == -(ActualTarget->GetActorLocation() - ControlledCharacter->GetActorLocation()).GetSafeNormal())
		{
			TargetRotation = FRotationMatrix::MakeFromX(-Direction).Rotator();
			
			GetWorldTimerManager().SetTimer(RotationTimerHandle, FTimerDelegate::CreateLambda([this, Direction, TargetRotation]()
			{
				CheckRotationFinished(-Direction, TargetRotation);
			}),0.01f, true);
		}
	}
}