#include "Pawn/EnemyPawn.h"


AEnemyPawn::AEnemyPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	SpotLightComponent = CreateDefaultSubobject<USpotLightComponent>(TEXT("SpotLight"));
	RootComponent = SpotLightComponent;
}

void AEnemyPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

void AEnemyPawn::Patrol()
{
	
}

