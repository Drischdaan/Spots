// Copyright Drischdaan All Rights Reserved Copyright

#include "SpotsEditorEditorModeCommands.h"

#define LOCTEXT_NAMESPACE "SpotsEditorEditorModeCommands"

FSpotsEditorEditorModeCommands::FSpotsEditorEditorModeCommands()
	: TCommands("SpotsEditorEditorMode",
	            NSLOCTEXT("SpotsEditorEditorMode", "SpotsEditorEditorModeCommands", "SpotsEditor Editor Mode"),
	            NAME_None,
	            FAppStyle::GetAppStyleSetName())
{
}

void FSpotsEditorEditorModeCommands::RegisterCommands()
{
	TArray<TSharedPtr<FUICommandInfo>>& ToolCommands = Commands.FindOrAdd(NAME_Default);

	UI_COMMAND(SimpleTool, "Show Actor Info", "Opens message box with info about a clicked actor", EUserInterfaceActionType::Button, FInputChord());
	ToolCommands.Add(SimpleTool);

	UI_COMMAND(InteractiveTool, "Measure Distance", "Measures distance between 2 points (click to set origin, shift-click to set end point)", EUserInterfaceActionType::ToggleButton, FInputChord());
	ToolCommands.Add(InteractiveTool);
}

TMap<FName, TArray<TSharedPtr<FUICommandInfo>>> FSpotsEditorEditorModeCommands::GetCommands()
{
	return Get().Commands;
}

#undef LOCTEXT_NAMESPACE
