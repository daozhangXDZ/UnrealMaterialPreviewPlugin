#pragma once
#include "CoreMinimal.h"
#include "IDZMaterialPreview.h"
#include "DZMaterialPreviewViewPort.h"
#include "Tickable.h"
#include "UObject/GCObject.h"
#include "Materials/Material.h"
#include "Widgets/SWidget.h"
#include "Widgets/Layout/SScrollBox.h"
#include "DZMaterialPreviewInfo.h"
#include "Misc/NotifyHook.h"

class FDZMaterialPreview : public IDZMaterialPreview, public FGCObject, public FTickableGameObject, public FNotifyHook
{
public:
	virtual void RegisterTabSpawners(const TSharedRef<class FTabManager>& TabManager) override;
	virtual void UnregisterTabSpawners(const TSharedRef<class FTabManager>& TabManager) override;
	
public:
	void InitMaterial(UMaterial* pInitMaterial);
	void InitMaterialEditor();

	/** Constructor */
	FDZMaterialPreview();

	virtual ~FDZMaterialPreview();

	/** FGCObject interface */
	virtual void AddReferencedObjects(FReferenceCollector& Collector) override;


	// FTickableGameObject interface
	virtual void Tick(float DeltaTime) override;

	virtual bool IsTickable() const override
	{
		return true;
	}

	virtual bool IsTickableWhenPaused() const override
	{
		return true;
	}

	virtual bool IsTickableInEditor() const override
	{
		return true;
	}

	TStatId GetStatId(void)  const override
	{
		RETURN_QUICK_DECLARE_CYCLE_STAT(FDZMaterialPreview, STATGROUP_Tickables);
	}
	UMaterial* Material;
	void GetShowHiddenParameters(bool& bShowHiddenParameters);


	virtual void NotifyPreChange(UProperty* PropertyAboutToChange) override;
	virtual void NotifyPostChange(const FPropertyChangedEvent& PropertyChangedEvent, UProperty* PropertyThatChanged)override;
protected:

	void UpdatePropertyWindow();
	/** Creates all internal widgets for the tabs to point at */
	void CreateInternalWidgets();

	void OnWindowsClose(TSharedRef<SDockTab> DockTab);


	//Refresh By Info
	void RefreshPreviewAsset();
	void SetPreviewMaterial(UMaterial* InMaterial);

public:
	UDZMaterialPreviewInfo* CurrPreviewMateriaInfo;

private:
	TSharedRef<SDockTab> SpawnTab_Preview(const FSpawnTabArgs& Args);
	TSharedRef<SDockTab> SpawnTab_Detail(const FSpawnTabArgs& Args);
	TSharedRef<SDockTab> SpawnTab_Tool(const FSpawnTabArgs& Args);
	void AddToSpawnedToolPanels(const FName& TabIdentifier, const TSharedRef<SDockTab>& SpawnedTab);


private:
	TSharedPtr<FWorkspaceItem> WorkspaceMenuCategory;
	/** List of open tool panels; used to ensure only one exists at any one time */
	TMap< FName, TWeakPtr<class SDockTab> > SpawnedToolPanels;


	/** Preview Viewport widget */
	TSharedPtr<class SDZMaterialPreviewViewPort> PreviewViewport;


	TSharedPtr<class IDetailsView> CurrPreviewMateriaInfoDetails;

	static const FName Preview_View_TabId;
	static const FName Preview_Detail_TabId;
	static const FName Preview_Tool_TabId;
};