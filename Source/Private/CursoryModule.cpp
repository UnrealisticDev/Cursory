// © 2021 Mustafa Moiz. All rights reserved.

#include "CursoryModule.h"
#include "CursorySystem.h"

DEFINE_LOG_CATEGORY(LogCursory);

class FCursoryModule : public ICursoryModule
{	
	void StartupModule() override;
	void ShutdownModule() override;

	UCursorySystem& GetSystem() override;
	
	UCursorySystem* CursorySystem{nullptr};
};

void FCursoryModule::StartupModule()
{
	CursorySystem = NewObject<UCursorySystem>(GetTransientPackage(), UCursorySystem::StaticClass(), NAME_None);
	CursorySystem->AddToRoot();
	CursorySystem->Init();
}

void FCursoryModule::ShutdownModule()
{
	if (CursorySystem)
	{
		CursorySystem->RemoveFromRoot();
	}
}

UCursorySystem& FCursoryModule::GetSystem()
{
	return *CursorySystem;
}

IMPLEMENT_GAME_MODULE(FCursoryModule, Cursory);
