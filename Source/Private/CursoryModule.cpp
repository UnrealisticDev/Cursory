// © 2021 Mustafa Moiz. All rights reserved.

#include "CursoryModule.h"
#include "CursoryGlobals.h"

DEFINE_LOG_CATEGORY(LogCursory);

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
	GetGlobals()->Init();
}

void FCursoryModule::ShutdownModule()
{
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
