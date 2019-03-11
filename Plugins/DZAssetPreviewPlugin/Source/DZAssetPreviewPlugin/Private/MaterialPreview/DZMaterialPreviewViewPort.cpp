#include "DZMaterialPreviewViewPort.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/SViewport.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/Layout/SBox.h"
#include "EditorStyleSet.h"
#include "Components/MeshComponent.h"
#include "Engine/SkeletalMesh.h"
#include "Editor/UnrealEdEngine.h"
#include "MaterialEditor/MaterialEditorMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Engine/Selection.h"
#include "Editor.h"
#include "Dialogs/Dialogs.h"
#include "UnrealEdGlobals.h"
#include "Slate/SceneViewport.h"
#include "Widgets/Docking/SDockTab.h"
#include "Engine/TextureCube.h"
#include "ComponentAssetBroker.h"
#include "SlateMaterialBrush.h"
#include "Widgets/Input/SNumericEntryBox.h"
#include "AdvancedPreviewScene.h"
#include "AssetViewerSettings.h"

#define LOCTEXT_NAMESPACE "DZMaterialPreview"

class IDZMaterialPreview;
class FDZMaterialPreviewViewportClient : public FEditorViewportClient
{
public:
	FDZMaterialPreviewViewportClient(TWeakPtr<IDZMaterialPreview> InMaterialEditor, FAdvancedPreviewScene& InPreviewScene, const TSharedRef<SDZMaterialPreviewViewPort>& InMaterialEditorViewport);

	// FEditorViewportClient interface
	virtual bool InputKey(FViewport* InViewport, int32 ControllerId, FKey Key, EInputEvent Event, float AmountDepressed = 1.f, bool bGamepad = false) override;
	virtual FLinearColor GetBackgroundColor() const override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void Draw(FViewport* Viewport, FCanvas* Canvas) override;
	virtual bool ShouldOrbitCamera() const override;

private:

	/** Pointer back to the material editor tool that owns us */
	TWeakPtr<IDZMaterialPreview> MaterialPreviewPtr;

	/** Preview Scene - uses advanced preview settings */
	class FAdvancedPreviewScene* AdvancedPreviewScene;
};

FDZMaterialPreviewViewportClient::FDZMaterialPreviewViewportClient(TWeakPtr<IDZMaterialPreview> InMaterialEditor, FAdvancedPreviewScene& InPreviewScene, const TSharedRef<SDZMaterialPreviewViewPort>& InMaterialEditorViewport)
	: FEditorViewportClient(nullptr, &InPreviewScene, StaticCastSharedRef<SEditorViewport>(InMaterialEditorViewport))
	, MaterialPreviewPtr(InMaterialEditor)
{
	DrawHelper.bDrawPivot = false;
	DrawHelper.bDrawWorldBox = false;
	DrawHelper.bDrawKillZ = false;
	DrawHelper.bDrawGrid = false;
	DrawHelper.GridColorAxis = FColor(80, 80, 80);
	DrawHelper.GridColorMajor = FColor(72, 72, 72);
	DrawHelper.GridColorMinor = FColor(64, 64, 64);
	DrawHelper.PerspectiveGridSize = HALF_WORLD_MAX1;

	SetViewMode(VMI_Lit);

	EngineShowFlags.DisableAdvancedFeatures();
	EngineShowFlags.SetSnap(0);
	EngineShowFlags.SetSeparateTranslucency(true);

	OverrideNearClipPlane(1.0f);
	bUsingOrbitCamera = true;

	// Don't want to display the widget in this viewport
	Widget->SetDefaultVisibility(false);

	AdvancedPreviewScene = &InPreviewScene;
}

bool FDZMaterialPreviewViewportClient::InputKey(FViewport* InViewport, int32 ControllerId, FKey Key, EInputEvent Event, float AmountDepressed /* = 1.f */, bool bGamepad /* = false */)
{
	return false;
}

FLinearColor FDZMaterialPreviewViewportClient::GetBackgroundColor() const
{
	return FLinearColor::White;
}

void FDZMaterialPreviewViewportClient::Tick(float DeltaSeconds)
{
	FEditorViewportClient::Tick(DeltaSeconds);

	// Tick the preview scene world.
	if (!GIntraFrameDebuggingGameThread)
	{
		PreviewScene->GetWorld()->Tick(LEVELTICK_All, DeltaSeconds);
	}
}

void FDZMaterialPreviewViewportClient::Draw(FViewport* InViewport, FCanvas* Canvas)
{
	FEditorViewportClient::Draw(InViewport, Canvas);
	//MaterialEditorPtr.Pin()->DrawMessages(InViewport, Canvas);
}

bool FDZMaterialPreviewViewportClient::ShouldOrbitCamera() const
{
	return false;
}


////////////////////////////////   PreviewViewPort ////////////////////////////////////

void SDZMaterialPreviewViewPort::Construct(const FArguments & InArgs)
{
	/*MaterialPreviewPtr = InArgs.DZMaterialPreview;*/
	AdvancedPreviewScene = MakeShareable(new FAdvancedPreviewScene(FPreviewScene::ConstructionValues()));

	//bShowGrid = false;
	//bShowBackground = false;
	//PreviewPrimType = TPT_None;

	SEditorViewport::Construct(SEditorViewport::FArguments());

	//PreviewMaterial = nullptr;
	//PreviewMeshComponent = nullptr;

	//UMaterialInterface* Material = MaterialEditorPtr.Pin()->GetMaterialInterface();
	//if (Material)
	//{
	//	SetPreviewMaterial(Material);
	//}
	SetPreviewMaterial(GUnrealEd->GetThumbnailManager()->FloorPlaneMaterial);
	SetPreviewAsset(GUnrealEd->GetThumbnailManager()->EditorSphere);
}

SDZMaterialPreviewViewPort::~SDZMaterialPreviewViewPort()
{

}


void SDZMaterialPreviewViewPort::AddReferencedObjects(FReferenceCollector & Collector)
{
}

void SDZMaterialPreviewViewPort::OnAddedToTab(const TSharedRef<SDockTab>& OwnerTab)
{
	ParentTab = OwnerTab;
}

TSharedRef<FEditorViewportClient> SDZMaterialPreviewViewPort::MakeEditorViewportClient()
{
	EditorViewportClient = MakeShareable(new FDZMaterialPreviewViewportClient(MaterialPreviewPtr, *AdvancedPreviewScene.Get(), SharedThis(this)));
	UAssetViewerSettings::Get()->OnAssetViewerSettingsChanged().AddRaw(this, &SDZMaterialPreviewViewPort::OnAssetViewerSettingsChanged);
	EditorViewportClient->SetViewLocation(FVector::ZeroVector);
	EditorViewportClient->SetViewRotation(FRotator(0.0f, -90.0f, 0.0f));
	EditorViewportClient->SetViewLocationForOrbiting(FVector::ZeroVector);
	EditorViewportClient->bSetListenerPosition = false;
	EditorViewportClient->EngineShowFlags.EnableAdvancedFeatures();
	EditorViewportClient->EngineShowFlags.SetLighting(true);
	EditorViewportClient->EngineShowFlags.SetIndirectLightingCache(true);
	EditorViewportClient->EngineShowFlags.SetPostProcessing(true);
	EditorViewportClient->Invalidate();
	EditorViewportClient->VisibilityDelegate.BindSP(this, &SDZMaterialPreviewViewPort::IsVisible);

	return EditorViewportClient.ToSharedRef();
}

void SDZMaterialPreviewViewPort::PopulateViewportOverlays(TSharedRef<class SOverlay> Overlay)
{
}

EVisibility SDZMaterialPreviewViewPort::OnGetViewportContentVisibility() const
{
	return EVisibility();
}

void SDZMaterialPreviewViewPort::BindCommands()
{
}

void SDZMaterialPreviewViewPort::OnFocusViewportToSelection()
{
}

/** Call back for when the user changes preview scene settings in the UI */
void SDZMaterialPreviewViewPort::OnAssetViewerSettingsChanged(const FName& InPropertyName)
{

}

bool SDZMaterialPreviewViewPort::IsVisible() const
{
	return ViewportWidget.IsValid() && (!ParentTab.IsValid() || ParentTab.Pin()->IsForeground()) && SEditorViewport::IsVisible();
}



// ICommonEditorViewportToolbarInfoProvider interface
TSharedRef<class SEditorViewport> SDZMaterialPreviewViewPort::GetViewportWidget()
{
	return SharedThis(this);
}
TSharedPtr<FExtender> SDZMaterialPreviewViewPort::GetExtenders() const
{
	TSharedPtr<FExtender> Result(MakeShareable(new FExtender));
	return Result;
}
void SDZMaterialPreviewViewPort::OnFloatingButtonClicked()
{

}


void SDZMaterialPreviewViewPort::RefreshViewport()
{

}

bool SDZMaterialPreviewViewPort::SetPreviewAsset(UObject * InAsset)
{
	/*if (!MaterialEditorPtr.Pin()->ApproveSetPreviewAsset(InAsset))
	{
		return false;
	}*/

	// Unregister the current component
	if (PreviewMeshComponent != nullptr)
	{
		AdvancedPreviewScene->RemoveComponent(PreviewMeshComponent);
		PreviewMeshComponent = nullptr;
	}

	FTransform Transform = FTransform::Identity;

	if (UStaticMesh* StaticMesh = Cast<UStaticMesh>(InAsset))
	{
		// Special case handling for static meshes, to use more accurate bounds via a subclass
		UStaticMeshComponent* NewSMComponent = NewObject<UMaterialEditorMeshComponent>(GetTransientPackage(), NAME_None, RF_Transient);
		NewSMComponent->SetStaticMesh(StaticMesh);

		PreviewMeshComponent = NewSMComponent;

		// Update the toolbar state implicitly through PreviewPrimType.
		if (StaticMesh == GUnrealEd->GetThumbnailManager()->EditorCylinder)
		{
			PreviewPrimType = TPT_Cylinder;
		}
		else if (StaticMesh == GUnrealEd->GetThumbnailManager()->EditorCube)
		{
			PreviewPrimType = TPT_Cube;
		}
		else if (StaticMesh == GUnrealEd->GetThumbnailManager()->EditorSphere)
		{
			PreviewPrimType = TPT_Sphere;
		}
		else if (StaticMesh == GUnrealEd->GetThumbnailManager()->EditorPlane)
		{
			PreviewPrimType = TPT_Plane;
		}
		else
		{
			PreviewPrimType = TPT_None;
		}
	}
	else if (InAsset != nullptr)
	{
		// Fall back to the component asset broker
		if (TSubclassOf<UActorComponent> ComponentClass = FComponentAssetBrokerage::GetPrimaryComponentForAsset(InAsset->GetClass()))
		{
			if (ComponentClass->IsChildOf(UMeshComponent::StaticClass()))
			{
				PreviewMeshComponent = NewObject<UMeshComponent>(GetTransientPackage(), ComponentClass, NAME_None, RF_Transient);

				FComponentAssetBrokerage::AssignAssetToComponent(PreviewMeshComponent, InAsset);

				PreviewPrimType = TPT_None;
			}
		}
	}

	// Add the new component to the scene
	if (PreviewMeshComponent != nullptr)
	{
		AdvancedPreviewScene->AddComponent(PreviewMeshComponent, Transform);
		AdvancedPreviewScene->SetFloorOffset(-PreviewMeshComponent->Bounds.Origin.Z + PreviewMeshComponent->Bounds.BoxExtent.Z);

	}

	// Make sure the preview material is applied to the component
	SetPreviewMaterial(PreviewMaterial);

	return (PreviewMeshComponent != nullptr);
}

void SDZMaterialPreviewViewPort::SetPreviewMaterial(UMaterial * InMaterial)
{
	if (InMaterial == nullptr)
	{

	}
	else
	{
		PreviewMaterial = InMaterial;
	}
	

	if (PreviewMeshComponent != nullptr)
	{
		PreviewMeshComponent->OverrideMaterials.Empty();
		PreviewMeshComponent->OverrideMaterials.Add(PreviewMaterial);
		PreviewMeshComponent->MarkRenderStateDirty();
	}
}
// End of ICommonEditorViewportToolbarInfoProvider interface

#undef LOCTEXT_NAMESPACE