// Copyright Drischdaan All Rights Reserved Copyright

#include "SpotsEditorEditorMode.h"
#include "InteractiveToolManager.h"
#include "SpotsEditorEditorModeCommands.h"
#include "SpotsEditorEditorModeToolkit.h"

#include "Data/SpotsData.h"

#include "Debug/DebugDrawService.h"

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

void USpotsEditorEditorMode::SetPreviewData(const TMap<FName, TArray<FSpotsData>>& InData)
{
	PreviewData = InData;
}

void USpotsEditorEditorMode::ClearPreviewData()
{
	PreviewData.Empty();
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

	DrawHandle = UDebugDrawService::Register(TEXT("Editor"), FDebugDrawDelegate::CreateUObject(this, &USpotsEditorEditorMode::DrawSpots));
}

void USpotsEditorEditorMode::Exit()
{
	UDebugDrawService::Unregister(DrawHandle);
	ClearPreviewData();

	Super::Exit();
}

void USpotsEditorEditorMode::CreateToolkit()
{
	Toolkit = MakeShareable(new FSpotsEditorEditorModeToolkit);
}

TMap<FName, TArray<TSharedPtr<FUICommandInfo>>> USpotsEditorEditorMode::GetModeCommands() const
{
	return FSpotsEditorEditorModeCommands::Get().GetCommands();
}

void USpotsEditorEditorMode::DrawSpots(UCanvas* Canvas, APlayerController* PlayerController)
{
	const UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	for (const auto& [VolumeId, Spots] : PreviewData)
	{
		for (const FSpotsData& Spot : Spots)
		{
			DrawDebugPoint(World, FVector(Spot.Position), 8.0f, FColor::Green, false, -1.0f, SDPG_Foreground);
		}
	}
}

#undef LOCTEXT_NAMESPACE
