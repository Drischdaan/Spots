// Copyright Drischdaan All Rights Reserved Copyright

#include "SpotsEditorEditorMode.h"
#include "InteractiveToolManager.h"
#include "SpotsEditorEditorModeCommands.h"
#include "SpotsEditorEditorModeToolkit.h"
#include "Modules/ModuleManager.h"

#include "Tools/SpotsEditorInteractiveTool.h"
#include "Tools/SpotsEditorSimpleTool.h"

#define LOCTEXT_NAMESPACE "SpotsEditorEditorMode"

const FEditorModeID USpotsEditorEditorMode::EM_SpotsEditorEditorModeId = TEXT("EM_SpotsEditorEditorMode");

FString USpotsEditorEditorMode::SimpleToolName = TEXT("SpotsEditor_ActorInfoTool");
FString USpotsEditorEditorMode::InteractiveToolName = TEXT("SpotsEditor_MeasureDistanceTool");


USpotsEditorEditorMode::USpotsEditorEditorMode()
{
	FModuleManager::Get().LoadModule("EditorStyle");

	Info = FEditorModeInfo(USpotsEditorEditorMode::EM_SpotsEditorEditorModeId,
	                       LOCTEXT("ModeName", "SpotsEditor"),
	                       FSlateIcon(),
	                       true);
}

USpotsEditorEditorMode::~USpotsEditorEditorMode()
{
}

void USpotsEditorEditorMode::ActorSelectionChangeNotify()
{
}

void USpotsEditorEditorMode::Enter()
{
	UEdMode::Enter();

	const FSpotsEditorEditorModeCommands& SampleToolCommands = FSpotsEditorEditorModeCommands::Get();

	RegisterTool(SampleToolCommands.SimpleTool, SimpleToolName, NewObject<USpotsEditorSimpleToolBuilder>(this));
	RegisterTool(SampleToolCommands.InteractiveTool, InteractiveToolName, NewObject<USpotsEditorInteractiveToolBuilder>(this));

	GetToolManager()->SelectActiveToolType(EToolSide::Left, SimpleToolName);
}

void USpotsEditorEditorMode::CreateToolkit()
{
	Toolkit = MakeShareable(new FSpotsEditorEditorModeToolkit);
}

TMap<FName, TArray<TSharedPtr<FUICommandInfo>>> USpotsEditorEditorMode::GetModeCommands() const
{
	return FSpotsEditorEditorModeCommands::Get().GetCommands();
}

#undef LOCTEXT_NAMESPACE
