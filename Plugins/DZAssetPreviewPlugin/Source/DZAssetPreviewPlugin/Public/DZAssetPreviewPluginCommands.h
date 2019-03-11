// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "DZAssetPreviewPluginStyle.h"

class FDZAssetPreviewPluginCommands : public TCommands<FDZAssetPreviewPluginCommands>
{
public:

	FDZAssetPreviewPluginCommands()
		: TCommands<FDZAssetPreviewPluginCommands>(TEXT("DZAssetPreviewPlugin"), NSLOCTEXT("Contexts", "DZAssetPreviewPlugin", "DZAssetPreviewPlugin Plugin"), NAME_None, FDZAssetPreviewPluginStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > OpenPluginWindow;
};