// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameInstance.h"

#include "Blueprint/UserWidget.h"
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
		UGameplayStatics::OpenLevel(World, *sceneName);



		if (sceneName == "Level_01")
		{
			Timer=0;
			if(!timerHandle.IsValid())
			{
				TimerManager->SetTimer(timerHandle, this, &UMyGameInstance::UpdateTimer, 1.f, true);
			}
		}
	}
}


void UMyGameInstance::StealItem()
{
	NbrItemToSteal--;
	if (NbrItemToSteal == 0)
	{
		PlayScene("EndGame");
	}
}

