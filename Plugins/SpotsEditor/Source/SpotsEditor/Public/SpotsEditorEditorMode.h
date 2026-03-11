// Copyright Drischdaan All Rights Reserved Copyright

#pragma once

#include "Tools/UEdMode.h"
#include "SpotsEditorEditorMode.generated.h"

UCLASS()
class USpotsEditorEditorMode : public UEdMode
{
	GENERATED_BODY()

public:
	USpotsEditorEditorMode();
	virtual ~USpotsEditorEditorMode() override;

protected:
	virtual void Enter() override;
	virtual void ActorSelectionChangeNotify() override;
	virtual void CreateToolkit() override;
	virtual TMap<FName, TArray<TSharedPtr<FUICommandInfo>>> GetModeCommands() const override;

public:
	const static FEditorModeID EM_SpotsEditorEditorModeId;

	static FString SimpleToolName;
	static FString InteractiveToolName;
};
