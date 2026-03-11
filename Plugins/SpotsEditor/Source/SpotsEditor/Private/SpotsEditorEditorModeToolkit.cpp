// Copyright Drischdaan All Rights Reserved Copyright

#include "SpotsEditorEditorModeToolkit.h"

#include "EditorModeManager.h"
#include "IDetailsView.h"
#include "PropertyEditorModule.h"

#include "Data/SpotsData.h"
#include "Generator/SpotsGenerator.h"

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

TSharedPtr<SWidget> FSpotsEditorEditorModeToolkit::GetInlineContent() const
{
	return SNew(SVerticalBox)
		+ SVerticalBox::Slot().AutoHeight().Padding(4.0f)
		[
			SNew(SButton)
			.Text(LOCTEXT("GenerateSpots", "Generate Spots"))
			.HAlign(HAlign_Center)
			.OnClicked_Lambda([this]()
			{
				UWorld* World = GEditor->GetEditorWorldContext().World();
				const TMap<FName, TArray<FSpotsData>> VolumeData = FSpotsGenerator::GenerateSpots(World);
				FSpotsGenerator::SaveToDataTables(World, VolumeData);

				USpotsEditorEditorMode* Mode = Cast<USpotsEditorEditorMode>(
					GLevelEditorModeTools().GetActiveScriptableMode(
						USpotsEditorEditorMode::EM_SpotsEditorEditorModeId));
				if (Mode)
				{
					Mode->SetPreviewData(VolumeData);
				}
				return FReply::Handled();
			})
		]
		+ SVerticalBox::Slot()
		.AutoHeight()
		[
			ModeDetailsView.ToSharedRef()
		]
		+ SVerticalBox::Slot()
		[
			DetailsView.ToSharedRef()
		];
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
