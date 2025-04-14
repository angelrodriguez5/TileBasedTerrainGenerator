// Copyright Epic Games, Inc. All Rights Reserved.

#include "TileBasedTerrainGeneratorEditorStyle.h"
#include "Styling/SlateStyleRegistry.h"
#include "Framework/Application/SlateApplication.h"
#include "Slate/SlateGameResources.h"
#include "Interfaces/IPluginManager.h"
#include "Styling/SlateStyleMacros.h"

#define RootToContentDir Style->RootToContentDir

TSharedPtr<FSlateStyleSet> FTileBasedTerrainGeneratorEditorStyle::StyleInstance = nullptr;

void FTileBasedTerrainGeneratorEditorStyle::Initialize()
{
	if (!StyleInstance.IsValid())
	{
		StyleInstance = Create();
		FSlateStyleRegistry::RegisterSlateStyle(*StyleInstance);
	}
}

void FTileBasedTerrainGeneratorEditorStyle::Shutdown()
{
	FSlateStyleRegistry::UnRegisterSlateStyle(*StyleInstance);
	ensure(StyleInstance.IsUnique());
	StyleInstance.Reset();
}

FName FTileBasedTerrainGeneratorEditorStyle::GetStyleSetName()
{
	static FName StyleSetName(TEXT("TileBasedTerrainGeneratorEditorStyle"));
	return StyleSetName;
}

const FVector2D Icon16x16(16.0f, 16.0f);
const FVector2D Icon20x20(20.0f, 20.0f);

TSharedRef< FSlateStyleSet > FTileBasedTerrainGeneratorEditorStyle::Create()
{
	TSharedRef< FSlateStyleSet > Style = MakeShareable(new FSlateStyleSet("TileBasedTerrainGeneratorEditorStyle"));
	Style->SetContentRoot(IPluginManager::Get().FindPlugin(TEXT("TileBasedTerrainGenerator"))->GetBaseDir() / TEXT("Resources"));

	Style->Set("TileBasedTerrainGeneratorEditor.OpenPluginWindow", new IMAGE_BRUSH_SVG(TEXT("PlaceholderButtonIcon"), Icon20x20));

	return Style;
}

void FTileBasedTerrainGeneratorEditorStyle::ReloadTextures()
{
	if (FSlateApplication::IsInitialized())
	{
		FSlateApplication::Get().GetRenderer()->ReloadTextureResources();
	}
}

const ISlateStyle& FTileBasedTerrainGeneratorEditorStyle::Get()
{
	return *StyleInstance;
}
