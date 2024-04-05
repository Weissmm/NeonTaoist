// Copyright Epic Games, Inc. All Rights Reserved.

#include "NeonTaoistGameMode.h"
#include "NeonTaoistCharacter.h"
#include "NeonTaoistGameState.h"
#include "Player/NeonTaoistPlayState.h"
#include "UObject/ConstructorHelpers.h"

ANeonTaoistGameMode::ANeonTaoistGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	GameStateClass = ANeonTaoistGameState::StaticClass();
}
