#pragma once
#include "CoreMinimal.h"
#include "SEditorViewport.h"
#include "IDZMaterialPreview.h"
#include "Components/MeshComponent.h"
#include "ThumbnailRendering/ThumbnailManager.h"
#include "PreviewScene.h"
#include "EditorViewportClient.h"
#include "Editor/UnrealEd/Public/SCommonEditorViewportToolbarBase.h"


class SDZMaterialPreviewViewPort : public SEditorViewport,public FGCObject, public ICommonEditorViewportToolbarInfoProvider
{
public:
	SLATE_BEGIN_ARGS(SDZMaterialPreviewViewPort) {}
	SLATE_ARGUMENT(TWeakPtr<IDZMaterialPreview>, DZMaterialPreview)
		SLATE_END_ARGS()

		void Construct(const FArguments& InArgs);
	~SDZMaterialPreviewViewPort();

	virtual void AddReferencedObjects(FReferenceCollector& Collector) override;

	void OnAddedToTab(const TSharedRef<SDockTab>& OwnerTab);

	// ICommonEditorViewportToolbarInfoProvider interface
	virtual TSharedRef<class SEditorViewport> GetViewportWidget() override;
	virtual TSharedPtr<FExtender> GetExtenders() const override;
	virtual void OnFloatingButtonClicked() override;
	// End of ICommonEditorViewportToolbarInfoProvider interface

	//Refresh
	void RefreshViewport();
	bool SetPreviewAsset(UObject* InAsset);
	void SetPreviewMaterial(UMaterial* InMaterial);


protected:
	/** SEditorViewport interface */
	virtual TSharedRef<FEditorViewportClient> MakeEditorViewportClient() override;
	virtual void PopulateViewportOverlays(TSharedRef<class SOverlay> Overlay) override;
	virtual EVisibility OnGetViewportContentVisibility() const override;
	virtual void BindCommands() override;
	virtual void OnFocusViewportToSelection() override;
	/** Call back for when the user changes preview scene settings in the UI */
	void OnAssetViewerSettingsChanged(const FName& InPropertyName);

	bool IsVisible() const;

public:
	/** Material for the preview mesh */
	UMaterial* PreviewMaterial;
	/** Component for the preview mesh. */
	UMeshComponent* PreviewMeshComponent;
private:
	/** The parent tab where this viewport resides */
	TWeakPtr<SDockTab> ParentTab;
private:
	TSharedPtr<class FDZMaterialPreviewViewportClient> EditorViewportClient;

	/** Pointer back to the material editor tool that owns us */
	TWeakPtr<IDZMaterialPreview> MaterialPreviewPtr;

	/** Preview Scene - uses advanced preview settings */
	TSharedPtr<class FAdvancedPreviewScene> AdvancedPreviewScene;

		/** The preview primitive we are using. */
	EThumbnailPrimType PreviewPrimType;
};