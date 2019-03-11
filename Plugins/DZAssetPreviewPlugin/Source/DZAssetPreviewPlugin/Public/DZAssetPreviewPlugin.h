// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ModuleManager.h"
#include "MaterialPreview/IDZMaterialPreview.h"
#include "Materials/Material.h"

class FToolBarBuilder;
class FMenuBuilder;

class FDZAssetPreviewPluginModule : public IModuleInterface, public IHasMenuExtensibility, public IHasToolBarExtensibility
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	void OpenMateriaPreview();
	void closeMaterialPreview(const TSharedRef<FTabManager> & pTabmanager);
	/** This function will be bound to Command (by default it will bring up plugin window) */
	void PluginButtonClicked();

	/** Gets the extensibility managers for outside entities to extend material editor's menus and toolbars */
	virtual TSharedPtr<FExtensibilityManager> GetMenuExtensibilityManager() override { return MenuExtensibilityManager; }
	virtual TSharedPtr<FExtensibilityManager> GetToolBarExtensibilityManager() override { return ToolBarExtensibilityManager; }
	//TSharedRef<SDockTab> OnSpawnPluginTab(const FSpawnTabArgs& SpawnTabArgs);
private:

	void AddToolbarExtension(FToolBarBuilder& Builder);
	void AddMenuExtension(FMenuBuilder& Builder);

private:
	TSharedPtr<IDZMaterialPreview> mCurrMateriePreview;
	TSharedPtr<class FUICommandList> PluginCommands;
private:
	TSharedPtr<FExtensibilityManager> MenuExtensibilityManager;
	TSharedPtr<FExtensibilityManager> ToolBarExtensibilityManager;
	bool isOpening = false;
};