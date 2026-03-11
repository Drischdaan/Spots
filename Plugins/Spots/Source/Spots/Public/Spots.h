// Copyright Drischdaan All Rights Reserved Copyright

#pragma once

#include "Modules/ModuleManager.h"

SPOTS_API DECLARE_LOG_CATEGORY_EXTERN(LogSpots, Log, All);

class FSpotsModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
