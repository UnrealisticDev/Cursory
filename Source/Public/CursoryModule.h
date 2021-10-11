// © 2021 Mustafa Moiz. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "Logging/LogMacros.h"

class UCursorySystem;

DECLARE_LOG_CATEGORY_EXTERN(LogCursory, Log, All);

/**
 * Public interface for Cursory module.
 * Provides advanced hardware cursor capabilities.
 */
class ICursoryModule : public FDefaultGameModuleImpl
{
public:

	static UCursorySystem& Get()
	{
		return FModuleManager::LoadModuleChecked<ICursoryModule>("Cursory").GetSystem();
	}

private:
	
	virtual UCursorySystem& GetSystem() = 0;
};