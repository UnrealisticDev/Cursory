// Copyright Mowgl33. All rights reserved.

#include "CursoryFunctionLibrary.h"
#include "GenericPlatform/ICursor.h"
#include "Modules/ModuleManager.h"
#include "CursoryModule.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "CursoryGlobals.h"
#include "Framework/Application/SlateApplication.h"
#include "Widgets/SViewport.h"
#include "Engine/World.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/Widget.h"

void UCursoryFunctionLibrary::UseStandardCursor(const UObject* WorldContextObject, EMouseCursor::Type Cursor)
{
	if (Cursor == EMouseCursor::Custom)
	{
		UE_LOG(LogCursory, Warning, TEXT("Used UseStandardCursor() to use custom cursor. Use UseCustomCursor() instead."));
		return;
	}

	APlayerController* PlayerOne = UGameplayStatics::GetPlayerController(WorldContextObject, 0);
	if (!PlayerOne)
	{
		UE_LOG(LogCursory, Warning, TEXT("Could not find player to use cursor on."));
		return;
	}

	PlayerOne->CurrentMouseCursor = Cursor;
}

void UCursoryFunctionLibrary::UseCustomCursor(const UObject* WorldContextObject, FGameplayTag Identifier)
{
	APlayerController* PlayerOne = UGameplayStatics::GetPlayerController(WorldContextObject, 0);
	if (!PlayerOne)
	{
		UE_LOG(LogCursory, Warning, TEXT("Could not find player to use cursor on."));
		return;
	}

	ICursoryModule::Get().GetGlobals()->MountCustomCursor(Identifier);
	PlayerOne->CurrentMouseCursor = EMouseCursor::Custom;
}

void UCursoryFunctionLibrary::UseWidgetStandardCursor(UWidget* Widget, EMouseCursor::Type Cursor)
{
	if (Widget)
	{
		Widget->SetCursor(Cursor);
	}
}

void UCursoryFunctionLibrary::UseWidgetCustomCursor(UWidget* Widget, FGameplayTag Identifier)
{
	if (Widget)
	{
		ICursoryModule::Get().GetGlobals()->MountCustomCursor(Identifier, true);
		Widget->SetCursor(EMouseCursor::Custom);
	}
}

void UCursoryFunctionLibrary::UsePlayerCursorForWidget(UWidget* Widget)
{
	if (Widget)
	{
		APlayerController* PlayerOne = UGameplayStatics::GetPlayerController(Widget, 0);
		if (!PlayerOne)
		{
			UE_LOG(LogCursory, Warning, TEXT("Could not find player to use cursor on."));
			return;
		}

		Widget->SetCursor(PlayerOne->GetMouseCursor());
	}
}

void UCursoryFunctionLibrary::PauseAutoFocusViewport()
{
	ICursoryModule::Get().GetGlobals()->SetAutoFocusViewport(true);
}

void UCursoryFunctionLibrary::ResumeAutoFocusViewport()
{
	ICursoryModule::Get().GetGlobals()->SetAutoFocusViewport(false);
}
