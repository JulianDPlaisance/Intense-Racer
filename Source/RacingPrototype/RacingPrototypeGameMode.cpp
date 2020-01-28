// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "RacingPrototypeGameMode.h"
#include "RacingPrototypePawn.h"
#include "RacingPrototypeHud.h"

ARacingPrototypeGameMode::ARacingPrototypeGameMode()
{
	DefaultPawnClass = ARacingPrototypePawn::StaticClass();
	HUDClass = ARacingPrototypeHud::StaticClass();
}
