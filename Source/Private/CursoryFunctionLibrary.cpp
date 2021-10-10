// © 2021 Mustafa Moiz. All rights reserved.

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
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetTree.h"

void UCursoryFunctionLibrary::UseStandardCursor(const UObject* WorldContextObject, EMouseCursor::Type Cursor)
{
	// Stub
}

void UCursoryFunctionLibrary::UseCustomCursor(const UObject* WorldContextObject, FGameplayTag Identifier)
{
	// Stub
}

void UCursoryFunctionLibrary::ResetBaseCursor()
{
	UseBaseStandardCursor(EMouseCursor::Default);
}

void UCursoryFunctionLibrary::UseBaseStandardCursor(EMouseCursor::Type Cursor)
{
	if (Cursor == EMouseCursor::Custom)
	{
		UE_LOG(LogCursory, Warning, TEXT("Used UseBaseStandardCursor() to set custom cursor. Use UseBaseCustomCursor() instead."));
		return;
	}

	FCursorStackElement NewBaseCursor;
	{
		NewBaseCursor.CursorType = Cursor;
	}

	ICursoryModule::Get().GetGlobals()->ModifyBaseCursor(NewBaseCursor);
}

void UCursoryFunctionLibrary::UseBaseCustomCursor(FGameplayTag Identifier)
{
	FCursorStackElement NewBaseCursor;
	{
		NewBaseCursor.CursorType = EMouseCursor::Custom;
		NewBaseCursor.CustomCursorIdentifier = Identifier;
	}

	ICursoryModule::Get().GetGlobals()->ModifyBaseCursor(NewBaseCursor);
}

FCursorStackElementHandle UCursoryFunctionLibrary::PushStandardCursor(EMouseCursor::Type Cursor)
{
	FCursorStackElement NewCursor(FCursorStackElementHandle::Generate());
	{
		NewCursor.CursorType = Cursor;
	}

	return ICursoryModule::Get().GetGlobals()->PushCursor(NewCursor);
}

FCursorStackElementHandle UCursoryFunctionLibrary::PushCustomCursor(FGameplayTag Identifier)
{
	FCursorStackElement NewCursor(FCursorStackElementHandle::Generate());
	{
		NewCursor.CursorType = EMouseCursor::Custom;
		NewCursor.CustomCursorIdentifier = Identifier;
	}

	return ICursoryModule::Get().GetGlobals()->PushCursor(NewCursor);
}

void UCursoryFunctionLibrary::SetStandardCursorByHandle(FCursorStackElementHandle Handle, EMouseCursor::Type Cursor)
{
	FCursorStackElement NewCursor;
	{
		NewCursor.CursorType = Cursor;
	}

	ICursoryModule::Get().GetGlobals()->ModifyCursorByHandle(Handle, NewCursor);
}

void UCursoryFunctionLibrary::SetCustomCursorByHandle(FCursorStackElementHandle Handle, FGameplayTag Identifier)
{
	FCursorStackElement NewCursor;
	{
		NewCursor.CursorType = EMouseCursor::Custom;
		NewCursor.CustomCursorIdentifier = Identifier;
	}

	ICursoryModule::Get().GetGlobals()->ModifyCursorByHandle(Handle, NewCursor);
}

void UCursoryFunctionLibrary::RemoveCursorByHandle(FCursorStackElementHandle Handle)
{
	ICursoryModule::Get().GetGlobals()->RemoveCursorByHandle(Handle);
}

void UCursoryFunctionLibrary::PopCursor()
{
	ICursoryModule::Get().GetGlobals()->PopCursor();
}

void UCursoryFunctionLibrary::ResetCursorStack()
{
	ICursoryModule::Get().GetGlobals()->ResetCursorStack();
}

void UCursoryFunctionLibrary::ConformSWidgetToCursory(SWidget* Widget)
{
	if (Widget)
	{
		TAttribute<TOptional<EMouseCursor::Type>> Callback;
		Callback.BindUObject(ICursoryModule::Get().GetGlobals(), &UCursoryGlobals::GetCurrentCursorType);
		Widget->SetCursor(Callback);
	}
}

void UCursoryFunctionLibrary::ConformWidgetToCursory(UWidget* Widget)
{
	if (Widget)
	{
		TSharedPtr<SWidget> UnderlyingWidget = Widget->GetCachedWidget();
		if (UnderlyingWidget.IsValid())
		{
			TAttribute<TOptional<EMouseCursor::Type>> Callback;
			Callback.BindUObject(ICursoryModule::Get().GetGlobals(), &UCursoryGlobals::GetCurrentCursorType);
			UnderlyingWidget->SetCursor(Callback);
		}
	}
}

void UCursoryFunctionLibrary::ConformWidgetsToCursory(TArray<UWidget*> Widgets)
{
	for (UWidget* Widget : Widgets)
	{
		ConformWidgetToCursory(Widget);
	}
}

void UCursoryFunctionLibrary::ConformWidgetToCursoryRecursive(UUserWidget* Widget, bool bDescendantUserWidgets /* = false */)
{
	if (Widget)
	{
		UWidgetTree* WidgetTree = Widget->WidgetTree;
		
		if (bDescendantUserWidgets)
		{
			WidgetTree->ForEachWidgetAndDescendants([](UWidget* InWidget)
			{
				ConformWidgetToCursory(InWidget);
			});
		}

		else
		{
			WidgetTree->ForEachWidget([](UWidget* InWidget)
			{
				ConformWidgetToCursory(InWidget);
			});
		}
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
