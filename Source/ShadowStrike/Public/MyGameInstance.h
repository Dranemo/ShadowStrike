// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MyGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class SHADOWSTRIKE_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:

	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Timer")
	int Timer;



	UFUNCTION(BlueprintCallable)
	void ResetScene();

	
	UFUNCTION(BlueprintCallable)
	void PlayScene(FString name);


	

private:
	virtual void Init() override;
	FTimerHandle timerHandle;

	void UpdateTimer();
	
};
