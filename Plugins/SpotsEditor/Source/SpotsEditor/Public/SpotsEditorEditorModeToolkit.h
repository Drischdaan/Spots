// Copyright Drischdaan All Rights Reserved Copyright

#pragma once

#include "SpotsEditorEditorMode.h"
#include "Toolkits/BaseToolkit.h"

class FSpotsEditorEditorModeToolkit : public FModeToolkit
{
public:
	FSpotsEditorEditorModeToolkit();

protected:
	virtual void Init(const TSharedPtr<IToolkitHost>& InitToolkitHost, TWeakObjectPtr<UEdMode> InOwningMode) override;
	virtual void GetToolPaletteNames(TArray<FName>& PaletteNames) const override;

	virtual TSharedPtr<SWidget> GetInlineContent() const override;

	virtual FName GetToolkitFName() const override;
	virtual FText GetBaseToolkitName() const override;
};
