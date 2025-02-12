#include "Character/AIEnemyController.h"

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
	FRotator TargetRotation = FRotationMatrix::MakeFromX(Direction).Rotator();
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, FString::Printf(TEXT("Move to location: %s"), *TargetRotation.ToString()));
	ControlledCharacter->SetActorRotation(FMath::RInterpTo(
		GetPawn()->GetActorRotation(), 
		FRotator(0, 0, TargetRotation.Yaw),                   
		 UGameplayStatics::GetWorldDeltaSeconds(this),  
		5.0f
	));

	GetWorldTimerManager().SetTimer(RotationTimerHandle, this, &AAIEnemyController::CheckRotationFinished, 0.1f, true);
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

void AAIEnemyController::CheckRotationFinished()
{
	if (!ControlledCharacter) return;

	FVector ForwardVector = ControlledCharacter->GetActorForwardVector();
	FVector Direction = (ActualTarget->GetActorLocation() - ControlledCharacter->GetActorLocation()).GetSafeNormal();
	
	float DotProduct = FVector::DotProduct(ForwardVector, Direction);
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, FString::Printf(TEXT("Move to location: %f"), DotProduct));
	if (DotProduct > 0.99f) 
	{
		GetWorld()->GetTimerManager().ClearTimer(RotationTimerHandle);
		MoveToNextLocation(); 
	}
}
