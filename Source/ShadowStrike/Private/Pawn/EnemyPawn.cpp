#include "Pawn/EnemyPawn.h"


AEnemyPawn::AEnemyPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	SpotLightComponent = CreateDefaultSubobject<USpotLightComponent>(TEXT("SpotLight"));
	SpotLightComponent->SetupAttachment(CapsuleComponent);
}


void AEnemyPawn::BeginPlay()
{
	Super::BeginPlay();
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("Pawn"));
	
}

void AEnemyPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

