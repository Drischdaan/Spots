// Copyright Drischdaan All Rights Reserved Copyright

#pragma once

#include "Modules/ModuleManager.h"

class FSpotsModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
