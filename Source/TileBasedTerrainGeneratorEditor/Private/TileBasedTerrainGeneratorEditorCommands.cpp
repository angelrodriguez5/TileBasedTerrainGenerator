// Copyright Epic Games, Inc. All Rights Reserved.

#include "TileBasedTerrainGeneratorEditorCommands.h"

#define LOCTEXT_NAMESPACE "FTileBasedTerrainGeneratorEditorModule"

void FTileBasedTerrainGeneratorEditorCommands::RegisterCommands()
{
	UI_COMMAND(OpenPluginWindow, "TileBasedTerrainGenerator", "Bring up TileBasedTerrainGeneratorEditor window", EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE
