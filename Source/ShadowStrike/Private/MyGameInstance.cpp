// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameInstance.h"

#include "Blueprint/UserWidget.h"
#include "Character/EnemyCharacter.h"
#include "Kismet/GameplayStatics.h"




void UMyGameInstance::Init()
{
	Super::Init();

	PlayScene("MainMenu");
}

void UMyGameInstance::UpdateTimer()
{
	Timer+=1;
}

void UMyGameInstance::ResetScene()
{
	UWorld* World = GetWorld();
	if (World)
	{
		FName CurrentLevel = *World->GetMapName();
		PlayScene(CurrentLevel.ToString());
	}
}



void UMyGameInstance::PlayScene(FString sceneName)
{
	UWorld* World = GetWorld();
	if (World)
	{

		FName CurrentLevel = *World->GetMapName();

		UGameplayStatics::OpenLevel(World, *sceneName);

		GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Green, sceneName + CurrentLevel.ToString());

		if (sceneName == "LevelFinal")
		{
			if(CurrentLevel.ToString() != sceneName)
			{
				UGameplayStatics::PlaySound2D(World, GameMusic, 1.f, 1.f, 0.f);

				if(!GameMusic)
					GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Purple, TEXT("PlayMusic"));
			}
			
			Timer=0;
			if(!timerHandle.IsValid())
			{
				TimerManager->SetTimer(timerHandle, this, &UMyGameInstance::UpdateTimer, 1.f, true);
			}
		}
		else
		{
			if(CurrentLevel.ToString() != "LevelFinal")
			{
				GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Purple, TEXT("PlayMusic"));	
				UGameplayStatics::PlaySound2D(World, MainMenuMusic, 1.f, 1.f, 0.f);
			}

			
			TimerManager->ClearTimer(timerHandle);
		}
	}
}


void UMyGameInstance::StealItem()
{
	NbrItemToSteal--;
	if (NbrItemToSteal == 0)
	{
		TArray<AActor*> FoundActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemyCharacter::StaticClass(), FoundActors);

		for (auto FoundActor : FoundActors)
		{
			Cast<AEnemyCharacter>(FoundActor)->DisbaleAllTimer();
		}

		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, "Stealed");
		PlayScene("EndGame");
	}
}

