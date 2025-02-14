// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuGameMode.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraActor.h"


AMainMenuGameMode::AMainMenuGameMode()
{
	DefaultPawnClass = nullptr;
}

void AMainMenuGameMode::StartPlay()
{
	Super::StartPlay();
	
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
    
	// Récupérer la Camera Actor placée dans le niveau
	ACameraActor* CameraActor = Cast<ACameraActor>(UGameplayStatics::GetActorOfClass(GetWorld(), ACameraActor::StaticClass()));
    
	if (PlayerController && CameraActor)
	{
		// Définir la caméra placée dans le monde comme cible de vue
		PlayerController->SetViewTargetWithBlend(CameraActor, 0.5f); // La valeur "0.5f" est la durée de la transition
	}
}
