// Copyright Epic Games, Inc. All Rights Reserved.

#include "TileBasedTerrainGeneratorEditor.h"
#include "TileBasedTerrainGeneratorEditorStyle.h"
#include "TileBasedTerrainGeneratorEditorCommands.h"
#include "LevelEditor.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "ToolMenus.h"

static const FName TileBasedTerrainGeneratorEditorTabName("TileBasedTerrainGeneratorEditor");

#define LOCTEXT_NAMESPACE "FTileBasedTerrainGeneratorEditorModule"

void FTileBasedTerrainGeneratorEditorModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FTileBasedTerrainGeneratorEditorStyle::Initialize();
	FTileBasedTerrainGeneratorEditorStyle::ReloadTextures();

	FTileBasedTerrainGeneratorEditorCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FTileBasedTerrainGeneratorEditorCommands::Get().OpenPluginWindow,
		FExecuteAction::CreateRaw(this, &FTileBasedTerrainGeneratorEditorModule::PluginButtonClicked),
		FCanExecuteAction());

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FTileBasedTerrainGeneratorEditorModule::RegisterMenus));
	
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(TileBasedTerrainGeneratorEditorTabName, FOnSpawnTab::CreateRaw(this, &FTileBasedTerrainGeneratorEditorModule::OnSpawnPluginTab))
		.SetDisplayName(LOCTEXT("FTileBasedTerrainGeneratorEditorTabTitle", "TileBasedTerrainGeneratorEditor"))
		.SetMenuType(ETabSpawnerMenuType::Hidden);
}

void FTileBasedTerrainGeneratorEditorModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	UToolMenus::UnRegisterStartupCallback(this);

	UToolMenus::UnregisterOwner(this);

	FTileBasedTerrainGeneratorEditorStyle::Shutdown();

	FTileBasedTerrainGeneratorEditorCommands::Unregister();

	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(TileBasedTerrainGeneratorEditorTabName);
}

TSharedRef<SDockTab> FTileBasedTerrainGeneratorEditorModule::OnSpawnPluginTab(const FSpawnTabArgs& SpawnTabArgs)
{
	FText WidgetText = FText::Format(
		LOCTEXT("WindowWidgetText", "Add code to {0} in {1} to override this window's contents"),
		FText::FromString(TEXT("FTileBasedTerrainGeneratorEditorModule::OnSpawnPluginTab")),
		FText::FromString(TEXT("TileBasedTerrainGeneratorEditor.cpp"))
		);

	return SNew(SDockTab)
		.TabRole(ETabRole::NomadTab)
		[
			// Put your tab content here!
			SNew(SBox)
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			[
				SNew(STextBlock)
				.Text(WidgetText)
			]
		];
}

void FTileBasedTerrainGeneratorEditorModule::PluginButtonClicked()
{
	FGlobalTabmanager::Get()->TryInvokeTab(TileBasedTerrainGeneratorEditorTabName);
}

void FTileBasedTerrainGeneratorEditorModule::RegisterMenus()
{
	// Owner will be used for cleanup in call to UToolMenus::UnregisterOwner
	FToolMenuOwnerScoped OwnerScoped(this);

	{
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");
		{
			FToolMenuSection& Section = Menu->FindOrAddSection("WindowLayout");
			Section.AddMenuEntryWithCommandList(FTileBasedTerrainGeneratorEditorCommands::Get().OpenPluginWindow, PluginCommands);
		}
	}

	{
		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar");
		{
			FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("Settings");
			{
				FToolMenuEntry& Entry = Section.AddEntry(FToolMenuEntry::InitToolBarButton(FTileBasedTerrainGeneratorEditorCommands::Get().OpenPluginWindow));
				Entry.SetCommandList(PluginCommands);
			}
		}
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FTileBasedTerrainGeneratorEditorModule, TileBasedTerrainGeneratorEditor)