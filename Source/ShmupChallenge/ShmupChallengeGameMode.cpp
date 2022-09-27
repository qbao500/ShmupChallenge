// Copyright Epic Games, Inc. All Rights Reserved.

#include "ShmupChallengeGameMode.h"
#include "ShmupChallengeCharacter.h"
#include "UObject/ConstructorHelpers.h"

AShmupChallengeGameMode::AShmupChallengeGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
