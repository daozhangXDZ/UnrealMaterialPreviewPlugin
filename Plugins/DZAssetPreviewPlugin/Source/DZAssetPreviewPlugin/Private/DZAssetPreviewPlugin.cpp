// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "DZAssetPreviewPlugin.h"
#include "DZAssetPreviewPluginStyle.h"
#include "DZAssetPreviewPluginCommands.h"
#include "LevelEditor.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"
#include "MaterialPreview/DZMaterialPreview.h"
#include "Toolkits/ToolkitManager.h"
#include "ConstructorHelpers.h"
#include "Editor/EditorWidgets/Public/SDropTarget.h"

static const FName DZAssetPreviewPluginTabName("DZAssetPreviewPlugin");

#define LOCTEXT_NAMESPACE "FDZAssetPreviewPluginModule"

void FDZAssetPreviewPluginModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	MenuExtensibilityManager = MakeShareable(new FExtensibilityManager);
	ToolBarExtensibilityManager = MakeShareable(new FExtensibilityManager);

	FDZAssetPreviewPluginStyle::Initialize();
	FDZAssetPreviewPluginStyle::ReloadTextures();

	FDZAssetPreviewPluginCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FDZAssetPreviewPluginCommands::Get().OpenPluginWindow,
		FExecuteAction::CreateRaw(this, &FDZAssetPreviewPluginModule::PluginButtonClicked),
		FCanExecuteAction());
		
	FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
	
	{
		TSharedPtr<FExtender> MenuExtender = MakeShareable(new FExtender());
		MenuExtender->AddMenuExtension("WindowLayout", EExtensionHook::After, PluginCommands, FMenuExtensionDelegate::CreateRaw(this, &FDZAssetPreviewPluginModule::AddMenuExtension));

		LevelEditorModule.GetMenuExtensibilityManager()->AddExtender(MenuExtender);
	}
	
	{
		TSharedPtr<FExtender> ToolbarExtender = MakeShareable(new FExtender);
		ToolbarExtender->AddToolBarExtension("Settings", EExtensionHook::After, PluginCommands, FToolBarExtensionDelegate::CreateRaw(this, &FDZAssetPreviewPluginModule::AddToolbarExtension));
		
		LevelEditorModule.GetToolBarExtensibilityManager()->AddExtender(ToolbarExtender);
	}

}




void FDZAssetPreviewPluginModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	FDZAssetPreviewPluginStyle::Shutdown();
	//if (!mCurrMateriePreview.IsValid())
	//{
	//	mCurrMateriePreview->UnregisterTabSpawners(FGlobalTabmanager::Get());
	//	mCurrMateriePreview.Reset();
	//}
	FDZAssetPreviewPluginCommands::Unregister();
}


void FDZAssetPreviewPluginModule::PluginButtonClicked()
{
	if (isOpening)
	{
		return;
	}
	else
	{
		OpenMateriaPreview();
	}
	
}



void FDZAssetPreviewPluginModule::AddMenuExtension(FMenuBuilder& Builder)
{
	Builder.AddMenuEntry(FDZAssetPreviewPluginCommands::Get().OpenPluginWindow);
}

void FDZAssetPreviewPluginModule::AddToolbarExtension(FToolBarBuilder& Builder)
{
	Builder.AddToolBarButton(FDZAssetPreviewPluginCommands::Get().OpenPluginWindow);
}


//////////////////////////////////////////////////////////////////////////EditorCreate////////////////////////////////////
void FDZAssetPreviewPluginModule::OpenMateriaPreview()
{
	isOpening = true;
	TSharedRef<FDZMaterialPreview> NewMaterialEditor(new FDZMaterialPreview());
	//NewMaterialEditor->InitMaterial(NewMaterialEditor->Material);
	NewMaterialEditor->InitMaterialEditor();
	NewMaterialEditor->RegisterTabSpawners(FGlobalTabmanager::Get());
	NewMaterialEditor->OnUnregisterTabSpawners().AddRaw(this, &FDZAssetPreviewPluginModule::closeMaterialPreview);
	mCurrMateriePreview = NewMaterialEditor;
}

void FDZAssetPreviewPluginModule::closeMaterialPreview(const TSharedRef<FTabManager> & pTabmanager)
{
	isOpening = false;
}


#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FDZAssetPreviewPluginModule, DZAssetPreviewPlugin)