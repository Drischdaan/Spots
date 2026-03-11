// Copyright Drischdaan All Rights Reserved Copyright

#include "SpotsEditorEditorModeToolkit.h"

#include "EditorModeManager.h"
#include "IDetailsView.h"
#include "PropertyEditorModule.h"
#include "Modules/ModuleManager.h"

#define LOCTEXT_NAMESPACE "SpotsEditorEditorModeToolkit"

FSpotsEditorEditorModeToolkit::FSpotsEditorEditorModeToolkit()
{
}

void FSpotsEditorEditorModeToolkit::Init(const TSharedPtr<IToolkitHost>& InitToolkitHost, const TWeakObjectPtr<UEdMode> InOwningMode)
{
	FModeToolkit::Init(InitToolkitHost, InOwningMode);
}

void FSpotsEditorEditorModeToolkit::GetToolPaletteNames(TArray<FName>& PaletteNames) const
{
	PaletteNames.Add(NAME_Default);
}


FName FSpotsEditorEditorModeToolkit::GetToolkitFName() const
{
	return FName("SpotsEditorEditorMode");
}

FText FSpotsEditorEditorModeToolkit::GetBaseToolkitName() const
{
	return LOCTEXT("DisplayName", "SpotsEditorEditorMode Toolkit");
}

#undef LOCTEXT_NAMESPACE
