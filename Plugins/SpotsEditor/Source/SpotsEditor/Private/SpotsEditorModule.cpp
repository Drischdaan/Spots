// Copyright Drischdaan All Rights Reserved Copyright

#include "SpotsEditorModule.h"
#include "SpotsEditorEditorModeCommands.h"

#define LOCTEXT_NAMESPACE "SpotsEditorModule"

void FSpotsEditorModule::StartupModule()
{
	FSpotsEditorEditorModeCommands::Register();
}

void FSpotsEditorModule::ShutdownModule()
{
	FSpotsEditorEditorModeCommands::Unregister();
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FSpotsEditorModule, SpotsEditor)
