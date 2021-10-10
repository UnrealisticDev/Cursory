// © 2021 Mustafa Moiz. All rights reserved.

#include "CursoryModule.h"
#include "CursoryGlobals.h"

DEFINE_LOG_CATEGORY(LogCursory);

class FCursoryModule : public ICursoryModule
{	
	void StartupModule() override;
	void ShutdownModule() override;

	UCursoryGlobals* GetGlobals() override;
	
	UCursoryGlobals* Globals{nullptr};
};

void FCursoryModule::StartupModule()
{
	Globals = NewObject<UCursoryGlobals>(GetTransientPackage(), UCursoryGlobals::StaticClass(), NAME_None);
	Globals->AddToRoot();
	Globals->Init();
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
	return Globals;
}

IMPLEMENT_GAME_MODULE(FCursoryModule, Cursory);
