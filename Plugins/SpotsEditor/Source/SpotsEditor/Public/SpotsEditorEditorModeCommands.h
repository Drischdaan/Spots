// Copyright Drischdaan All Rights Reserved Copyright

#pragma once

#include "Framework/Commands/Commands.h"

class FSpotsEditorEditorModeCommands : public TCommands<FSpotsEditorEditorModeCommands>
{
public:
	FSpotsEditorEditorModeCommands();

public:
	virtual void RegisterCommands() override;

public:
	static TMap<FName, TArray<TSharedPtr<FUICommandInfo>>> GetCommands();

public:
	TSharedPtr<FUICommandInfo> SimpleTool;
	TSharedPtr<FUICommandInfo> InteractiveTool;

protected:
	TMap<FName, TArray<TSharedPtr<FUICommandInfo>>> Commands;
};
