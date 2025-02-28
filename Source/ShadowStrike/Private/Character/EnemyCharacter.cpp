#include "Character/EnemyCharacter.h"

#include "VectorTypes.h"
#include "Actor/AK.h"
#include "Character/AIEnemyController.h"
#include "Kismet/GameplayStatics.h"


AEnemyCharacter::AEnemyCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	SpotLightComponent = CreateDefaultSubobject<USpotLightComponent>(TEXT("SpotLight"));
	SpotLightComponent->SetupAttachment(RootComponent);

	FiringWeaponLocation = CreateDefaultSubobject<USceneComponent>(TEXT("Firing Weapon Location"));
	FiringWeaponLocation->SetupAttachment(GetMesh());
}




void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	PlayerPawnCharacter = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));

	if(Weapons[0])
	{
		WeaponEquipped = GetWorld()->SpawnActor<AAK>(Weapons[0]);
		AddWeapon(WeaponEquipped);
		RifleCooldown = 0.2;
	}
}

void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GetIsDead())
	{
		return;
	}
	
	if (!PlayerDetected && !PlayerPawnCharacter->IsHidden && CheckPlayerDetection())
	{
		PlayerDetected = true;
		
		SpotLightComponent->SetLightColor(FColor::Red);

		UGameplayStatics::PlaySoundAtLocation(this, GuardDetectedPlayerSound, GetActorLocation());
		
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
		GetWorldTimerManager().SetTimer(RifleTransformTimerHandle, FTimerDelegate::CreateLambda([this]()
		{
			if (FiringWeaponLocation == nullptr)
			{
				GetWorldTimerManager().ClearTimer(RifleTransformTimerHandle);
			}
			else
			{
				SetRifleFiringTransform(FiringWeaponLocation->GetComponentLocation(), FiringWeaponLocation->GetComponentRotation());
			}
		}), 0.01f, true);
		GetWorldTimerManager().SetTimer(FiringCooldownHandle, this, &AEnemyCharacter::Fire, RifleCooldown, true);
	}
	else
	{
		PlayerDetected = false;
	}
}

void AEnemyCharacter::Fire()
{
	if(GetIsDead())
		return;

	
	if (CheckPlayerDetection())
	{
		if (!LookAtPlayerHandle.IsValid())
		{
			GetWorldTimerManager().SetTimer(LookAtPlayerHandle, this, &AEnemyCharacter::LookAtPlayer, 0.01f, true);
		}
		
		Super::Fire();
	}
	else
	{
		GetWorldTimerManager().SetTimer(RifleTransformTimerHandle, FTimerDelegate::CreateLambda([this]()
		{
			if (WeaponLocation == nullptr)
			{
				GetWorldTimerManager().ClearTimer(RifleTransformTimerHandle);
			}
			else
			{
				SetRifleFiringTransform(WeaponLocation->GetComponentLocation(), WeaponLocation->GetComponentRotation());
			}
		}), 0.01f, true);
		
		GetWorldTimerManager().ClearTimer(FiringCooldownHandle);
		PlayerDetected = false;
	}
}


void AEnemyCharacter::SetRifleFiringTransform(FVector TargetLocation, FRotator TargetRotation)
{
	if (GetIsDead() || WeaponEquipped == nullptr)
	{
		GetWorldTimerManager().ClearTimer(RifleTransformTimerHandle);
		return;
	}
	
	WeaponEquipped->SetActorLocation(FMath::VInterpTo(
	WeaponEquipped->GetActorLocation(), 
		 TargetLocation,                   
		 UGameplayStatics::GetWorldDeltaSeconds(this), 
		5.0f
	));

	WeaponEquipped->SetActorRotation(FMath::RInterpTo(
	WeaponEquipped->GetActorRotation(), 
		 TargetRotation,                   
		 UGameplayStatics::GetWorldDeltaSeconds(this), 
		5.0f
	));

	if (FVector::Dist(WeaponEquipped->GetActorLocation(), TargetLocation) < 1.0f &&
	FMath::Abs(WeaponEquipped->GetActorRotation().Yaw - TargetRotation.Yaw) < 1.0f)
	{
		GetWorldTimerManager().ClearTimer(RifleTransformTimerHandle);
	}
}

void AEnemyCharacter::LookAtPlayer()
{
	FVector Direction = (PlayerPawnCharacter->GetActorLocation() - GetActorLocation()).GetSafeNormal();
	FRotator TargetRotation = FRotationMatrix::MakeFromX(Direction).Rotator();
	
	SetActorRotation(FMath::RInterpTo(
		GetActorRotation(),
			TargetRotation,                   
			UGameplayStatics::GetWorldDeltaSeconds(this), 
		5.0f
		));

	if (FMath::Abs(GetActorRotation().Yaw - TargetRotation.Yaw) < 1.0f)
	{
		GetWorldTimerManager().ClearTimer(LookAtPlayerHandle);
	}
}




void AEnemyCharacter::Die()
{
	DisbaleAllTimer();

	SpotLightComponent->SetIntensity(0);
	
	DropWeapon();
	Super::Die();
}

void AEnemyCharacter::DisbaleAllTimer()
{
	GetWorldTimerManager().ClearTimer(FiringCooldownHandle);
	GetWorldTimerManager().ClearTimer(RifleTransformTimerHandle);
	GetWorldTimerManager().ClearTimer(LookAtPlayerHandle);

	AAIEnemyController* AIController = Cast<AAIEnemyController>(GetController());

	if (AIController)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, "AI Controller!");
		AIController->DisableAllTimer();
	}

}



