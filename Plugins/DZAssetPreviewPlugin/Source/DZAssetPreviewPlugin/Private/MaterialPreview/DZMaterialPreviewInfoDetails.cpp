#include "DZMaterialPreviewInfoDetails.h"
#include "Misc/MessageDialog.h"
#include "Misc/Guid.h"
#include "UObject/UnrealType.h"
#include "Layout/Margin.h"
#include "Misc/Attribute.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Text/STextBlock.h"
#include "EditorStyleSet.h"
#include "EditorSupportDelegates.h"
#include "DetailWidgetRow.h"
#include "PropertyHandle.h"
#include "IDetailPropertyRow.h"
#include "DetailLayoutBuilder.h"
#include "IDetailGroup.h"
#include "DetailCategoryBuilder.h"
#include "PropertyCustomizationHelpers.h"
#include "ScopedTransaction.h"
#include "DZMaterialPreviewInfo.h"
#define LOCTEXT_NAMESPACE "DZMaterial Priview"

TSharedRef<class IDetailCustomization> FDZMaterialPreviewInfoDetails::MakeInstance(UDZMaterialPreviewInfo * MaterialInfo)
{
	return MakeShareable(new FDZMaterialPreviewInfoDetails(MaterialInfo));
}

FDZMaterialPreviewInfoDetails::FDZMaterialPreviewInfoDetails(UDZMaterialPreviewInfo * MaterialInstance)
	: MaterialPreviewInfo(MaterialInstance)
{

}

void FDZMaterialPreviewInfoDetails::CustomizeDetails(IDetailLayoutBuilder & DetailLayout)
{
	FName GroupsCategoryName = TEXT("ParameterGroups");
	IDetailCategoryBuilder& GroupsCategory = DetailLayout.EditCategory(GroupsCategoryName, LOCTEXT("MICParamGroupsTitle", "Parameter Groups"));
	TSharedRef<IPropertyHandle> ParameterGroupsProperty = DetailLayout.GetProperty("ParameterGroups");
	/*SNew(SVerticalBox)
		+ SVerticalBox::Slot().AutoHeight().HAlign(HAlign_Left).Padding(1)
		[
			SNew(STextBlock).AutoWrapText(true).Text(LOCTEXT("Actor Transform Tool", "Actor Transform Tool"))
		]
	+ SVerticalBox::Slot().AutoHeight().HAlign(HAlign_Center).Padding(2)
		[
			SNew(STextBlock).AutoWrapText(true).Text(LOCTEXT("-------------------", "-------------------"))
		];*/
}
#undef LOCTEXT_NAMESPACE
