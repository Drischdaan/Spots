// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "UObject/NoExportTypes.h"
#include "InteractiveToolBuilder.h"
#include "BaseTools/SingleClickTool.h"
#include "SpotsEditorSimpleTool.generated.h"

/**
 * Builder for USpotsEditorSimpleTool
 */
UCLASS()
class SPOTSEDITOR_API USpotsEditorSimpleToolBuilder : public UInteractiveToolBuilder
{
	GENERATED_BODY()

public:
	virtual bool CanBuildTool(const FToolBuilderState& SceneState) const override { return true; }
	virtual UInteractiveTool* BuildTool(const FToolBuilderState& SceneState) const override;
};



/**
 * Settings UObject for USpotsEditorSimpleTool. This UClass inherits from UInteractiveToolPropertySet,
 * which provides an OnModified delegate that the Tool will listen to for changes in property values.
 */
UCLASS(Transient)
class SPOTSEDITOR_API USpotsEditorSimpleToolProperties : public UInteractiveToolPropertySet
{
	GENERATED_BODY()
public:
	USpotsEditorSimpleToolProperties();

	/** If enabled, dialog should display extended information about the actor clicked on. Otherwise, only basic info will be shown. */
	UPROPERTY(EditAnywhere, Category = Options, meta = (DisplayName = "Show Extended Info"))
	bool ShowExtendedInfo;
};




/**
 * USpotsEditorSimpleTool is an example Tool that opens a message box displaying info about an actor that the user
 * clicks left mouse button. All the action is in the ::OnClicked handler.
 */
UCLASS()
class SPOTSEDITOR_API USpotsEditorSimpleTool : public USingleClickTool
{
	GENERATED_BODY()

public:
	USpotsEditorSimpleTool();

	virtual void SetWorld(UWorld* World);

	virtual void Setup() override;

	virtual void OnClicked(const FInputDeviceRay& ClickPos);


protected:
	UPROPERTY()
	TObjectPtr<USpotsEditorSimpleToolProperties> Properties;


protected:
	/** target World we will raycast into to find actors */
	UWorld* TargetWorld;
};