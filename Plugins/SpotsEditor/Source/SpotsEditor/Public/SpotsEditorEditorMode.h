// Copyright Drischdaan All Rights Reserved Copyright

#pragma once

#include "Data/SpotsData.h"

#include "Tools/UEdMode.h"
#include "SpotsEditorEditorMode.generated.h"

UCLASS()
class USpotsEditorEditorMode : public UEdMode
{
	GENERATED_BODY()

public:
	USpotsEditorEditorMode();
	virtual ~USpotsEditorEditorMode() override;

public:
	void SetPreviewData(const TMap<FName, TArray<FSpotsData>>& InData);
	void ClearPreviewData();

protected:
	virtual void Enter() override;
	virtual void Exit() override;

	virtual void ActorSelectionChangeNotify() override;
	virtual void CreateToolkit() override;
	virtual TMap<FName, TArray<TSharedPtr<FUICommandInfo>>> GetModeCommands() const override;

protected:
	void DrawSpots(UCanvas* Canvas, APlayerController* PlayerController);

public:
	const static FEditorModeID EM_SpotsEditorEditorModeId;

	static FString SimpleToolName;
	static FString InteractiveToolName;

protected:
	FDelegateHandle DrawHandle;
	TMap<FName, TArray<FSpotsData>> PreviewData;
};
