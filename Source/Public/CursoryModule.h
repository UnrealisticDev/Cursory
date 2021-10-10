// © 2021 Mustafa Moiz. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "Logging/LogMacros.h"

class UCursoryGlobals;

DECLARE_LOG_CATEGORY_EXTERN(LogCursory, Log, All);

/**
 * Public interface for Cursory module.
 * Provides advanced hardware cursor capabilities.
 */
class ICursoryModule : public FDefaultGameModuleImpl
{
public:
	
	static ICursoryModule& Get()
	{
		return FModuleManager::LoadModuleChecked<ICursoryModule>("Cursory");
	}

	virtual UCursoryGlobals* GetGlobals() = 0;
};