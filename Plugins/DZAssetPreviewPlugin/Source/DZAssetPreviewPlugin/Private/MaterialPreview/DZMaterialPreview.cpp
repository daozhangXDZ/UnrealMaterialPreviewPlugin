#include "DZMaterialPreview.h"
#include "SlateOptMacros.h"
#include "Widgets/SWidget.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Layout/SSeparator.h"
#include "Widgets/Input/SButton.h"
#include "EditorStyleSet.h"
#include "UObject/Object.h"
#include "UObject/UObjectBaseUtility.h"
#include "Editor.h"
#include "SNodePanel.h"
#include "EditorClassUtils.h"
#include "DZAssetPreviewPlugin.h"
#include "ConstructorHelpers.h"
#include "PropertyEditorModule.h"
#include "DZMaterialPreviewInfoDetails.h"


#define LOCTEXT_NAMESPACE "DZMaterial Priview"

const FName FDZMaterialPreview::Preview_View_TabId(TEXT("DZMaterialPriview_ViewPortTab"));
const FName FDZMaterialPreview::Preview_Detail_TabId(TEXT("DZMaterialPriview_DetailTab"));
const FName FDZMaterialPreview::Preview_Tool_TabId(TEXT("DZMaterialPriview_ToolTab"));
const FName DZAssetPreview_MaterialPreview_Identifier = FName(TEXT("DZAssetPreview_MaterialPreview_App"));

FDZMaterialPreview::FDZMaterialPreview()
{

}

FDZMaterialPreview::~FDZMaterialPreview()
{

}

void FDZMaterialPreview::InitMaterial(UMaterial* pInitMaterial)
{
	Material = pInitMaterial;
}

void FDZMaterialPreview::InitMaterialEditor()
{
	CurrPreviewMateriaInfo = NewObject<UDZMaterialPreviewInfo>(GetTransientPackage(), NAME_None, RF_Transactional);
	CreateInternalWidgets();
}

void FDZMaterialPreview::RegisterTabSpawners(const TSharedRef<class FTabManager>& InTabManager)
{
	WorkspaceMenuCategory = InTabManager->AddLocalWorkspaceMenuCategory(LOCTEXT("WorkspaceMenu_DZMaterialPriview", "DZMaterial Priview"));
	auto WorkspaceMenuCategoryRef = WorkspaceMenuCategory.ToSharedRef();

	InTabManager->RegisterTabSpawner(Preview_View_TabId, FOnSpawnTab::CreateSP(this, &FDZMaterialPreview::SpawnTab_Preview))
		.SetDisplayName(LOCTEXT("DZMateriaPreviewViewportTab", "DZMateriaPreview"))
		.SetGroup(WorkspaceMenuCategoryRef);
	OnRegisterTabSpawners().Broadcast(InTabManager);

	InTabManager->InvokeTab(Preview_View_TabId)->SetOnTabClosed(SDockTab::FOnTabClosedCallback::CreateLambda([this](TSharedRef<SDockTab> Self) {
		this->OnWindowsClose(Self);
	}));;
}

void FDZMaterialPreview::OnWindowsClose(TSharedRef<SDockTab> DockTab)
{
	if (PreviewViewport.IsValid())
	{
		UnregisterTabSpawners(DockTab.Get().GetTabManager());
	}
}

TSharedRef<SDockTab> FDZMaterialPreview::SpawnTab_Preview(const FSpawnTabArgs& Args)
{
	if (Args.GetTabId().TabType != Preview_View_TabId)
	{

	}
	TSharedRef<SDockTab> SpawnedTab =
		SNew(SDockTab)
		.Label(LOCTEXT("DZMateriaPreviewTab", "DZMateriaPreview"))
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot().AutoHeight().HAlign(HAlign_Left).Padding(1)
		[
			SNew(SBox).MaxDesiredWidth(300).MaxDesiredHeight(300)
			[
				PreviewViewport.ToSharedRef()
			]
		]
			+ SVerticalBox::Slot().AutoHeight().HAlign(HAlign_Left).Padding(2)
				[
					SNew(SBox).MaxDesiredWidth(300).MaxDesiredHeight(300)
					[
						CurrPreviewMateriaInfoDetails.ToSharedRef()
					]				
				]
			
		];
	UpdatePropertyWindow();
	AddToSpawnedToolPanels(Args.GetTabId().TabType, SpawnedTab);
	return SpawnedTab;
}

void FDZMaterialPreview::UpdatePropertyWindow()
{
	TArray<UObject*> SelectedObjects;
	SelectedObjects.Add(CurrPreviewMateriaInfo);
	CurrPreviewMateriaInfoDetails->SetObjects(SelectedObjects, true);
}

void FDZMaterialPreview::AddToSpawnedToolPanels(const FName& TabIdentifier, const TSharedRef<SDockTab>& SpawnedTab)
{
	TWeakPtr<SDockTab>* TabSpot = SpawnedToolPanels.Find(TabIdentifier);
	if (!TabSpot)
	{
		SpawnedToolPanels.Add(TabIdentifier, SpawnedTab);
	}
	else
	{
		check(!TabSpot->IsValid());
		*TabSpot = SpawnedTab;
	}
}


void FDZMaterialPreview::UnregisterTabSpawners(const TSharedRef<class FTabManager>& InTabManager)
{
	InTabManager->UnregisterTabSpawner(Preview_View_TabId);
	InTabManager->UnregisterTabSpawner(Preview_Detail_TabId);
	InTabManager->UnregisterTabSpawner(Preview_Tool_TabId);
	OnUnregisterTabSpawners().Broadcast(InTabManager);
}

/** FGCObject interface */
void FDZMaterialPreview::AddReferencedObjects(FReferenceCollector& Collector)
{

}


// FTickableGameObject interface
void FDZMaterialPreview::Tick(float DeltaTime)
{

}

void FDZMaterialPreview::CreateInternalWidgets()
{
	PreviewViewport = SNew(SDZMaterialPreviewViewPort).DZMaterialPreview(SharedThis(this));

	FPropertyEditorModule& vpropertyEditorModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");
	const FDetailsViewArgs DetailsViewArgs(false, false, true, FDetailsViewArgs::ObjectsUseNameArea, true,this);
	CurrPreviewMateriaInfoDetails = vpropertyEditorModule.CreateDetailView(DetailsViewArgs);
	FOnGetDetailCustomizationInstance LayoutMICDetails = FOnGetDetailCustomizationInstance::CreateStatic
	(
		&FDZMaterialPreviewInfoDetails::MakeInstance, CurrPreviewMateriaInfo
	);
	CurrPreviewMateriaInfoDetails->RegisterInstancedCustomPropertyLayout(UDZMaterialPreviewInfo::StaticClass(), LayoutMICDetails);
}

void FDZMaterialPreview::GetShowHiddenParameters(bool& bShowHiddenParameters)
{
	
}


////////////////////////////////////////////////////////Refresh by Info




void FDZMaterialPreview::NotifyPreChange(UProperty * PropertyAboutToChange)
{

}

void FDZMaterialPreview::NotifyPostChange(const FPropertyChangedEvent & PropertyChangedEvent, UProperty * PropertyThatChanged)
{
	RefreshPreviewAsset();
}

void FDZMaterialPreview::RefreshPreviewAsset()
{
	SetPreviewMaterial(CurrPreviewMateriaInfo->showMaterial);
}


void FDZMaterialPreview::SetPreviewMaterial(UMaterial* InMaterial)
{
	if (PreviewViewport.IsValid())
	{
		PreviewViewport->SetPreviewMaterial(InMaterial);
	}
}

#undef LOCTEXT_NAMESPACE