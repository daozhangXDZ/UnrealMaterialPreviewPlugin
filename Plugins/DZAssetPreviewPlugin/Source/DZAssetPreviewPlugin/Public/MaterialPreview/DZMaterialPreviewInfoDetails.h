#pragma once
#include "CoreMinimal.h"
#include "Types/SlateEnums.h"
#include "Layout/Visibility.h"
#include "IDetailCustomization.h"

class UDZMaterialPreviewInfo;
DECLARE_DELEGATE_OneParam(FDZGetShowHiddenParameters, bool&);

class FDZMaterialPreviewInfoDetails : public IDetailCustomization
{
public:
	/** Makes a new instance of this detail layout class for a specific detail view requesting it */
	static TSharedRef<class IDetailCustomization> MakeInstance(UDZMaterialPreviewInfo* MaterialInfo);

	/** Constructor */
	FDZMaterialPreviewInfoDetails(UDZMaterialPreviewInfo* MaterialInstance);

	/** IDetailCustomization interface */
	virtual void CustomizeDetails(IDetailLayoutBuilder& DetailLayout) override;

private:
	UDZMaterialPreviewInfo* MaterialPreviewInfo;
	/** Delegate to call to determine if hidden parameters should be shown */
	//FDZGetShowHiddenParameters ShowHiddenDelegate;
};