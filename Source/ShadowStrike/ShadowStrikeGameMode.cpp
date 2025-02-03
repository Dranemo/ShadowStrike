// Copyright Epic Games, Inc. All Rights Reserved.

#include "ShadowStrikeGameMode.h"
#include "ShadowStrikeCharacter.h"
#include "UObject/ConstructorHelpers.h"

AShadowStrikeGameMode::AShadowStrikeGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
