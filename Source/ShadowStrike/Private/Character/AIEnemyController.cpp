#include "Character/AIEnemyController.h"

#include "InteractiveToolManager.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"


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

	
	if(!TurnAnimStarted && ControlledCharacter->LeftTurnAnimMontage)
	{
		ControlledCharacter->PlayAnimMontage(ControlledCharacter->LeftTurnAnimMontage);
		
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
		else
		{
			TargetRotation = FRotationMatrix::MakeFromX(-Direction).Rotator();
			
			GetWorldTimerManager().SetTimer(RotationTimerHandle, FTimerDelegate::CreateLambda([this, Direction, TargetRotation]()
			{
				CheckRotationFinished(-Direction, TargetRotation);
			}),0.01f, true);
		}
	}
}