// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/PlayerCharacter.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Actor/Knife.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Windows/WindowsApplication.h"

APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring arm"));
	SpringArm->SetupAttachment(RootComponent);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	CapsuleCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("SphereCollision"));
	CapsuleCollision->SetupAttachment(GetCapsuleComponent());
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	PlayerController =  Cast<APlayerController>(GetController());

	if (PlayerController)
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(PlayerMappingContext, 0);
		}

		PlayerController->bShowMouseCursor= true;


		if(Weapons[0])
		{
			WeaponEquipped = GetWorld()->SpawnActor<AKnife>(Weapons[0]);
			AddWeapon(WeaponEquipped);
		}
	}
}



void APlayerCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (MovementAction)
		{
			EnhancedInputComponent->BindAction(MovementAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);
		}

		if (FireAction)
		{
			EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Started, this, &APlayerCharacter::Fire);
		}

		if (HideAction)
		{
			EnhancedInputComponent->BindAction(HideAction, ETriggerEvent::Started, this, &APlayerCharacter::Hide);
		}
		
		if (PickUpAction)
		{
			EnhancedInputComponent->BindAction(PickUpAction, ETriggerEvent::Started, this, &APlayerCharacter::Pickup);
		}
	}
}

void APlayerCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);


	if(IsHidden)
		return;
	
	if (PlayerController)
	{
		FHitResult HitResult;
		PlayerController->GetHitResultUnderCursor(
			ECC_Visibility,
			false,
			HitResult);


		//GEngine->AddOnScreenDebugMessage(-1, 0.1f, FColor::Yellow, TEXT("RotateTurret"));
		Rotate(HitResult.Location);
	}
}


void APlayerCharacter::Move(const FInputActionValue& Value)
{
	
	if (IsHidden)
		return;

	const FVector2d MovementVector = Value.Get<FVector2d>();
	GEngine->AddOnScreenDebugMessage(-1, 0.1f, FColor::Yellow, FString::Printf(TEXT("%s"), *MovementVector.ToString()));
	
	if (Controller)
	{
		FVector DeltaLocation = FVector::ZeroVector;
		DeltaLocation.X = MovementVector.Y * Speed * UGameplayStatics::GetWorldDeltaSeconds(this);
		DeltaLocation.Y = MovementVector.X * Speed * UGameplayStatics::GetWorldDeltaSeconds(this);
		AddActorLocalOffset(DeltaLocation, true);
	}
}

void APlayerCharacter::Fire()
{
	
	if (IsHidden)
		return;
	
	Super::Fire();

	
}

void APlayerCharacter::Hide()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Hide");

	
	TArray<AActor*> OverlappingActors;
	
	CapsuleCollision->GetOverlappingActors(OverlappingActors);

	if (IsHidden)
	{
		IsHidden = false;
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "UnHide");
		
		return;
	}

	
	for (const auto OverlappingActor : OverlappingActors)
	{
		if (OverlappingActor && OverlappingActor->Tags.Contains(FName("Wall")))
		{
			IsHidden = true;
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Hiding"));
			
			return;
		}
	}

	
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Can't Hide");
}

void APlayerCharacter::Pickup()
{
	TArray<AActor*> OverlappingActors;
	

	if (IsHidden)
	{
		return;
	}

	
	CapsuleCollision->GetOverlappingActors(OverlappingActors);
	for (const auto OverlappingActor : OverlappingActors)
	{
		if (ABaseWeapon* WeaponFound = Cast<ABaseWeapon>(OverlappingActor))
		{
			if(WeaponFound != WeaponEquipped)
			{
				GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, TEXT("PICKUP Weapon"));
				AddWeapon(WeaponFound);
				return;
			}
		}
	}

	
	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, TEXT("nothing to PICKUP"));
}


void APlayerCharacter::Die()
{
	Super::Die();
}
