#include "Character/EnemyCharacter.h"

#include "Actor/AK.h"
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
	
	PlayerPawnCharacter = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));

	if(Weapons[0])
	{
		WeaponEquipped = GetWorld()->SpawnActor<AAK>(Weapons[0]);
		AddWeapon(WeaponEquipped);
	}
}

void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (!PlayerDetected && !PlayerPawnCharacter->IsHidden && CheckPlayerDetection())
	{
		PlayerDetected = true;
		
		SpotLightComponent->SetLightColor(FColor::Red);
		
		if (DownToAimAnimMontage)
		{
			PlayAnimMontage(DownToAimAnimMontage);
		}
		
		GetWorldTimerManager().SetTimer(DetectionDelayTimerHandle,this, &AEnemyCharacter::WaitDetectionDelay, 1.0f, false);
	}
	else if (!PlayerDetected)
	{
		SpotLightComponent->SetLightColor(FColor::White);
	}
}

bool AEnemyCharacter::CheckPlayerDetection()
{
	if (!SpotLightComponent || !SpotLightComponent->IsVisible())
	{
		return false;
	}
	
	if (PlayerPawnCharacter)
	{
		FVector PlayerLocation = PlayerPawnCharacter->GetActorLocation();
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
			
			if (bHit && HitResult.GetActor() == PlayerPawnCharacter)
			{
				return true;
			}
		}
	}
	return false;
}

void AEnemyCharacter::WaitDetectionDelay()
{
	GetWorldTimerManager().ClearTimer(DetectionDelayTimerHandle);
	if (CheckPlayerDetection())
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Player Detected2");
		if (FiringAnimMontage)
		{
			UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
			if (AnimInstance)
			{
				AnimInstance->Montage_Play(FiringAnimMontage);
				
				AnimInstance->Montage_SetNextSection(FName("LoopEnd"), FName("LoopStart"), FiringAnimMontage);
				
				GetWorldTimerManager().SetTimer(FiringCooldownHandle, this, &AEnemyCharacter::Fire, 1.0f, true);
			}
		}
	}
	else
	{
		PlayerDetected = false;;
	}
}

void AEnemyCharacter::Fire()
{

	if (CheckPlayerDetection())
	{
		Super::Fire();
	}
	else
	{
		PlayerDetected = false;
	}
}



