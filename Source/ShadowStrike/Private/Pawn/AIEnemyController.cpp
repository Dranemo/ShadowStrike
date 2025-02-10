#include "Pawn/AIEnemyController.h"
#include "Kismet/GameplayStatics.h"
#include "NavigationSystem.h"

AAIEnemyController::AEnemyPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	SpotLightComponent = CreateDefaultSubobject<USpotLightComponent>(TEXT("SpotLight"));
	RootComponent = SpotLightComponent;
}

void AAIEnemyController::BeginPlay()
{
	AIEnemyController::BeginPlay();
	
}

void AAIEnemyController::Patrol()
{
	
}