#pragma once
#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "Materials/Material.h"
#include "DZMaterialPreviewInfo.generated.h"

UCLASS(hidecategories = Object, collapsecategories)
class DZASSETPREVIEWPLUGIN_API UDZMaterialPreviewInfo:public UObject
{
	GENERATED_UCLASS_BODY()
public:
	UPROPERTY(EditAnywhere, Category = DZMaterialPreview_DetailInfo)
		UMaterial* showMaterial;
	UPROPERTY(EditAnywhere, Category = DZMaterialPreview_DetailInfo)
		float refreshTime;

};