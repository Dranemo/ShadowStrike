// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameInstance.h"

void UMyGameInstance::Init()
{
	Super::Init();

	if(!timerHandle.IsValid())
	{
		TimerManager->SetTimer(timerHandle, this, &UMyGameInstance::UpdateTimer, 1.f, true);
	}
}

void UMyGameInstance::UpdateTimer()
{
	Timer+=1;
}
