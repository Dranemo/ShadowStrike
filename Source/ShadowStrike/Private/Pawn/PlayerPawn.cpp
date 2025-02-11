// Fill out your copyright notice in the Description page of Project Settings.

#include "Pawn/PlayerPawn.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"

APlayerPawn::APlayerPawn()
{
	PrimaryActorTick.bCanEverTick = true;
	
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring arm"));
	SpringArm->SetupAttachment(RootComponent);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	SphereCollision->SetupAttachment(CapsuleComponent);
}

void APlayerPawn::BeginPlay()
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
	}
}



void APlayerPawn::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (MovementAction)
		{
			EnhancedInputComponent->BindAction(MovementAction, ETriggerEvent::Triggered, this, &APlayerPawn::Move);
		}

		if (FireAction)
		{
			EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Started, this, &APlayerPawn::Fire);
		}

		if (HideAction)
		{
			EnhancedInputComponent->BindAction(HideAction, ETriggerEvent::Started, this, &APlayerPawn::Hide);
		}
	}
}

void APlayerPawn::Tick(float DeltaSeconds)
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


void APlayerPawn::Move(const FInputActionValue& Value)
{
	
	if (IsHidden)
		return;

	const FVector2d MovementVector = Value.Get<FVector2d>();
	GEngine->AddOnScreenDebugMessage(-1, 0.1f, FColor::Yellow, FString::Printf(TEXT("%s"), *MovementVector.ToString()));
	
	if (Controller)
	{
		FVector DeltaLocation = FVector::ZeroVector;
		DeltaLocation.X = MovementVector.Y * Speed * UGameplayStatics::GetWorldDeltaSeconds(this);
		AddActorLocalOffset(DeltaLocation, true);

		FRotator DeltaRotation = FRotator::ZeroRotator;
		DeltaLocation.Y = MovementVector.X * Speed * UGameplayStatics::GetWorldDeltaSeconds(this);
		AddActorLocalOffset(DeltaLocation, true);
	}
}

void APlayerPawn::Fire()
{
	
	if (IsHidden)
		return;
	
	Super::Fire();

	
}

void APlayerPawn::Hide()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Hide");

	
	TArray<AActor*> OverlappingActors;
	
	SphereCollision->GetOverlappingActors(OverlappingActors);

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


void APlayerPawn::Die()
{
	Super::Die();
}