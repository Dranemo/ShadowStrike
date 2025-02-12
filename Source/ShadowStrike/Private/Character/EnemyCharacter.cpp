#include "Character/EnemyCharacter.h"
#include "Kismet/GameplayStatics.h"


AEnemyCharacter::AEnemyCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	SpotLightComponent = CreateDefaultSubobject<USpotLightComponent>(TEXT("SpotLight"));
	SpotLightComponent->SetupAttachment(RootComponent);
}


void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	
	PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
}

void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (CheckPlayerDetection())
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, "Player Detected");
	}
}

bool AEnemyCharacter::CheckPlayerDetection()
{
	if (!SpotLightComponent || !SpotLightComponent->IsVisible())
	{
		return false;
	}
	
	if (PlayerPawn)
	{
		FVector PlayerLocation = PlayerPawn->GetActorLocation();
		FVector ToCharacter = (PlayerLocation - SpotLightComponent->GetComponentLocation()).GetSafeNormal();
		float Distance = FVector(PlayerLocation - SpotLightComponent->GetComponentLocation()).Size();
		FVector LightDirection = SpotLightComponent->GetForwardVector();
		
		float DotProduct = FVector::DotProduct(LightDirection, ToCharacter);
		float Angle = FMath::Acos(DotProduct) * (180.f / PI);

		if (Angle <= SpotLightComponent->OuterConeAngle && Distance <= SpotLightComponent->AttenuationRadius)
		{
			FHitResult HitResult;
			FCollisionQueryParams QueryParams;
			QueryParams.AddIgnoredActor(this);

			bool bHit = GetWorld()->LineTraceSingleByChannel(
				HitResult,
				SpotLightComponent->GetComponentLocation(),
				PlayerLocation,
				ECC_Visibility,
				QueryParams
			);
			
			
			if (!bHit && HitResult.GetActor() == PlayerPawn)
			{
				return true;
			}
		}
	}
	return false;
}




