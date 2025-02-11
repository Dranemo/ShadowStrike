#include "Character/BaseCharacter.h"
#include "Kismet/GameplayStatics.h"

ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	
	
}

void ABaseCharacter::Die()
{
	GEngine->AddOnScreenDebugMessage(-1, 0.1f, FColor::Yellow, TEXT("Die"));
}


void ABaseCharacter::Fire()
{
}

void ABaseCharacter::Rotate(FVector LookAtTarget)
{
	USkeletalMeshComponent* CharacterMeshMesh = GetMesh();
	
		FVector ToTarget = LookAtTarget - CharacterMeshMesh->GetComponentLocation();
		FRotator LookAtRotation =  FRotator(0.f, ToTarget.Rotation().Yaw - 90, 0.f);

		CharacterMeshMesh->SetWorldRotation(
			FMath::RInterpTo(
				CharacterMeshMesh->GetComponentRotation(),
				LookAtRotation,
				UGameplayStatics::GetWorldDeltaSeconds(this),
				InterpSpeed));
}
