#pragma once

#include "CoreMinimal.h"
#include "Toolkits/AssetEditorToolkit.h"
#include "DZAssetPreviewPlugin.h"
class FMaterialRenderProxy;
class FViewport;

class IDZMaterialPreview : public TSharedFromThis<IDZMaterialPreview>
{
public:
	IDZMaterialPreview() {}
	virtual ~IDZMaterialPreview() {}
	virtual void RegisterTabSpawners(const TSharedRef<class FTabManager>& TabManager) {}
	virtual void UnregisterTabSpawners(const TSharedRef<class FTabManager>& TabManager) {}
	/** Delegate to be called when the tabs are being registered **/
	DECLARE_EVENT_OneParam(FDZAssetPreviewPluginModule, FRegisterTabSpawnersEvent, const TSharedRef<class FTabManager>&);
	virtual FRegisterTabSpawnersEvent& OnRegisterTabSpawners() { return RegisterTabSpawnersEvent; };

	/** Delegate to be called when the tabs are being unregistered **/
	DECLARE_EVENT_OneParam(FDZAssetPreviewPluginModule, FUnregisterTabSpawnersEvent, const TSharedRef<class FTabManager>&);
	virtual FUnregisterTabSpawnersEvent& OnUnregisterTabSpawners() { return UnregisterTabSpawnersEvent; };

private:
	FRegisterTabSpawnersEvent RegisterTabSpawnersEvent;
	FUnregisterTabSpawnersEvent UnregisterTabSpawnersEvent;
};