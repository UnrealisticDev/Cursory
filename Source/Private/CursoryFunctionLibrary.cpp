// Copyright Mowgl33. All rights reserved.

#include "CursoryFunctionLibrary.h"
#include "GenericPlatform/ICursor.h"
#include "Modules/ModuleManager.h"
#include "CursoryModule.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "CursoryGlobals.h"

void UCursoryFunctionLibrary::UseStandardCursor(const UObject* WorldContextObject, EMouseCursor::Type Cursor)
{
	if (Cursor == EMouseCursor::Custom)
	{
		UE_LOG(LogCursory, Warning, TEXT("Used UseStandardCursor() to use custom cursor. Use UseCustomCursor() instead."));
		return;
	}

	auto playerOne = UGameplayStatics::GetPlayerController(WorldContextObject, 0);
	if (!playerOne)
	{
		UE_LOG(LogCursory, Warning, TEXT("Could not find player to use cursor on."));
		return;
	}

	playerOne->CurrentMouseCursor = Cursor;
}

void UCursoryFunctionLibrary::UseCustomCursor(const UObject* WorldContextObject, FGameplayTag Identifier)
{
	auto playerOne = UGameplayStatics::GetPlayerController(WorldContextObject, 0);
	if (!playerOne)
	{
		UE_LOG(LogCursory, Warning, TEXT("Could not find player to use cursor on."));
		return;
	}

	ICursoryModule::Get().GetGlobals()->MountCustomCursor(Identifier);
	playerOne->CurrentMouseCursor = EMouseCursor::Custom;
}
