#include "Pawn/BasePawn.h"
#include "Kismet/GameplayStatics.h"

ABasePawn::ABasePawn()
{
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Component"));
	RootComponent = CapsuleComponent;

	ArrowComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow Component"));
	ArrowComponent->SetupAttachment(CapsuleComponent);

	CharacterMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Character Mesh"));
	CharacterMesh->SetupAttachment(CapsuleComponent);

	
	CharacterMovementComponent = CreateDefaultSubobject<UCharacterMovementComponent>(TEXT("Character Movement"));
}

void ABasePawn::BeginPlay()
{
	Super::BeginPlay();

	
	
}


void ABasePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABasePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ABasePawn::Die()
{
	GEngine->AddOnScreenDebugMessage(-1, 0.1f, FColor::Yellow, TEXT("Die"));
}


void ABasePawn::Fire()
{
}

void ABasePawn::Rotate(FVector LookAtTarget)
{
		FVector ToTarget = LookAtTarget - CharacterMesh->GetComponentLocation();
		FRotator LookAtRotation =  FRotator(0.f, ToTarget.Rotation().Yaw - 90, 0.f);

		CharacterMesh->SetWorldRotation(
			FMath::RInterpTo(
				CharacterMesh->GetComponentRotation(),
				LookAtRotation,
				UGameplayStatics::GetWorldDeltaSeconds(this),
				InterpSpeed));
}
