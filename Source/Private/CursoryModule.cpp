// © 2021 Mustafa Moiz. All rights reserved.

#include "CursoryModule.h"
#include "CursoryGlobals.h"

DEFINE_LOG_CATEGORY(LogCursory);

#define LOCTEXT_NAMESPACE "CursoryModule"

/**********************************************************************************
*	FCursoryModule
*	-------------
*	The private implementation of the Cursory module.
***********************************************************************************/
class FCursoryModule : public ICursoryModule
{
	void StartupModule() override;
	void ShutdownModule() override;

	UCursoryGlobals* GetGlobals() override;

private:

	UCursoryGlobals* Globals;
};

void FCursoryModule::StartupModule()
{
	UE_LOG(LogCursory, Log, TEXT("Cursory module loaded."));

	// Initialize globals.
	GetGlobals()->Init();
}

void FCursoryModule::ShutdownModule()
{
	UE_LOG(LogCursory, Log, TEXT("Cursory module unloaded."));

	// Uninitialize globals.
	if (Globals)
	{
		Globals->RemoveFromRoot();
	}
}

UCursoryGlobals* FCursoryModule::GetGlobals()
{
	if (!Globals)
	{
		Globals = NewObject<UCursoryGlobals>(GetTransientPackage(), UCursoryGlobals::StaticClass(), NAME_None);
		Globals->AddToRoot();
	}

	check(Globals);
	return Globals;
}

IMPLEMENT_GAME_MODULE(FCursoryModule, Cursory);

#undef LOCTEXT_NAMESPACE