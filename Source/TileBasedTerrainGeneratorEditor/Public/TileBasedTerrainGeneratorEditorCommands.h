// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Framework/Commands/Commands.h"
#include "TileBasedTerrainGeneratorEditorStyle.h"

class FTileBasedTerrainGeneratorEditorCommands : public TCommands<FTileBasedTerrainGeneratorEditorCommands>
{
public:

	FTileBasedTerrainGeneratorEditorCommands()
		: TCommands<FTileBasedTerrainGeneratorEditorCommands>(TEXT("TileBasedTerrainGeneratorEditor"), NSLOCTEXT("Contexts", "TileBasedTerrainGeneratorEditor", "TileBasedTerrainGeneratorEditor Plugin"), NAME_None, FTileBasedTerrainGeneratorEditorStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > OpenPluginWindow;
};