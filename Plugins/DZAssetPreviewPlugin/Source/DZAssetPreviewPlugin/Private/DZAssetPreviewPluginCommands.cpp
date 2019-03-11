// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "DZAssetPreviewPluginCommands.h"

#define LOCTEXT_NAMESPACE "FDZAssetPreviewPluginModule"

void FDZAssetPreviewPluginCommands::RegisterCommands()
{
	UI_COMMAND(OpenPluginWindow, "DZAssetPreviewPlugin", "Bring up DZAssetPreviewPlugin window", EUserInterfaceActionType::Button, FInputGesture());
}

#undef LOCTEXT_NAMESPACE
