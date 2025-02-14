#include "Character/BaseCharacter.h"
#include "Actor/BaseWeapon.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Actor/Knife.h"
#include "AssetTypeActions/AssetDefinition_SoundBase.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"

ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	WeaponLocation = CreateDefaultSubobject<USceneComponent>(TEXT("Spawn Projectile Location"));
	WeaponLocation->SetupAttachment(GetMesh());
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ABaseCharacter::Die()
{
	IsDead = true;
	GetMesh()->SetEnableGravity(false);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	UGameplayStatics::PlaySoundAtLocation(this, DeathSound, this->GetActorLocation());
	
	GEngine->AddOnScreenDebugMessage(-1, 0.1f, FColor::Yellow, TEXT("Die"));
}

ABaseWeapon* ABaseCharacter::GetWeapon()
{
	return WeaponEquipped;
}

void ABaseCharacter::AddWeapon(ABaseWeapon* Weapon)
{
	if(WeaponEquipped)
		DropWeapon();
	
	WeaponEquipped = Weapon;
	Weapon->SetOwner(this);

	WeaponEquipped->SetLight(false);

	FAttachmentTransformRules WeaponTransformRules = FAttachmentTransformRules(EAttachmentRule::KeepRelative, EAttachmentRule::KeepRelative, EAttachmentRule::KeepRelative, false);
	Weapon->AttachToComponent(GetMesh(), WeaponTransformRules);


	FTransform transformWeapon(WeaponLocation->GetComponentRotation(), WeaponLocation->GetComponentLocation(), Weapon->GetActorScale());
	Weapon->SetActorTransform(transformWeapon);

	ResetWeaponCooldown();

	UGameplayStatics::PlaySoundAtLocation(this, Weapon->PickedSound, GetActorLocation());
}

void ABaseCharacter::DropWeapon()
{
	WeaponEquipped->SetOwner(nullptr);

	WeaponEquipped->SetLight(true);

	FDetachmentTransformRules WeaponTransformRules = FDetachmentTransformRules(EDetachmentRule::KeepWorld, EDetachmentRule::KeepWorld, EDetachmentRule::KeepWorld, false);
	WeaponEquipped->DetachFromActor(WeaponTransformRules);
	
	FVector newLocation(WeaponEquipped->GetActorLocation().X, WeaponEquipped->GetActorLocation().Y , 10.f);
	WeaponEquipped->SetActorLocation(newLocation);
	WeaponEquipped->SetActorRotation(FRotator(0, 0, 0));

	WeaponEquipped = nullptr;
}


void ABaseCharacter::Fire()
{
	if(WeaponEquipped && WeaponCooldown == 0)
	{
		WeaponEquipped->Fire();
		WeaponCooldown = 1;

		if(AKnife* knife = Cast<AKnife>(WeaponEquipped))
		{
			if(!HandleCooldownWeapon.IsValid())
				GetWorld()->GetTimerManager().SetTimer(HandleCooldownWeapon, this, &ABaseCharacter::ResetWeaponCooldown, KnifeCooldown);
			
			if(KnifeAnim)
			{
				GEngine->AddOnScreenDebugMessage(-1, .1, FColor::Red, ("KnifeAnim"));
				PlayAnimMontage(KnifeAnim); 
			}
		}
		else
		{
			if(!HandleCooldownWeapon.IsValid())
				GetWorld()->GetTimerManager().SetTimer(HandleCooldownWeapon, this, &ABaseCharacter::ResetWeaponCooldown, RifleCooldown);
			
			if(RifleAnim)
			{
				GEngine->AddOnScreenDebugMessage(-1, .1, FColor::Red, ("RifleAnim"));
				PlayAnimMontage(RifleAnim);
			}
		}
	}
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

void ABaseCharacter::ResetWeaponCooldown()
{
	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Blue, TEXT("ResetCooldown"));
	WeaponCooldown = 0;
	GetWorldTimerManager().ClearTimer(HandleCooldownWeapon);
}
