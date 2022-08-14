// © 2021 Mustafa Moiz. All rights reserved.

#include "CursoryFunctionLibrary.h"
#include "GenericPlatform/ICursor.h"
#include "CursoryModule.h"
#include "GameFramework/PlayerController.h"
#include "CursorySystem.h"
#include "Widgets/SViewport.h"
#include "Components/Widget.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetTree.h"

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

	ICursoryModule::Get().ModifyBaseCursor(NewBaseCursor);
}

void UCursoryFunctionLibrary::UseBaseCustomCursor(FGameplayTag Identifier)
{
	FCursorStackElement NewBaseCursor;
	{
		NewBaseCursor.CursorType = EMouseCursor::Custom;
		NewBaseCursor.CustomCursorIdentifier = Identifier;
	}

	ICursoryModule::Get().ModifyBaseCursor(NewBaseCursor);
}

FCursorStackElementHandle UCursoryFunctionLibrary::PushStandardCursor(EMouseCursor::Type Cursor)
{
	FCursorStackElement NewCursor(FCursorStackElementHandle::Generate());
	{
		NewCursor.CursorType = Cursor;
	}

	return ICursoryModule::Get().PushCursor(NewCursor);
}

FCursorStackElementHandle UCursoryFunctionLibrary::PushCustomCursor(FGameplayTag Identifier)
{
	FCursorStackElement NewCursor(FCursorStackElementHandle::Generate());
	{
		NewCursor.CursorType = EMouseCursor::Custom;
		NewCursor.CustomCursorIdentifier = Identifier;
	}

	return ICursoryModule::Get().PushCursor(NewCursor);
}

void UCursoryFunctionLibrary::SetStandardCursorByHandle(FCursorStackElementHandle Handle, EMouseCursor::Type Cursor)
{
	FCursorStackElement NewCursor;
	{
		NewCursor.CursorType = Cursor;
	}

	ICursoryModule::Get().ModifyCursorByHandle(Handle, NewCursor);
}

void UCursoryFunctionLibrary::SetCustomCursorByHandle(FCursorStackElementHandle Handle, FGameplayTag Identifier)
{
	FCursorStackElement NewCursor;
	{
		NewCursor.CursorType = EMouseCursor::Custom;
		NewCursor.CustomCursorIdentifier = Identifier;
	}

	ICursoryModule::Get().ModifyCursorByHandle(Handle, NewCursor);
}

void UCursoryFunctionLibrary::RemoveCursorByHandle(FCursorStackElementHandle Handle)
{
	ICursoryModule::Get().RemoveCursorByHandle(Handle);
}

void UCursoryFunctionLibrary::PopCursor()
{
	ICursoryModule::Get().PopCursor();
}

void UCursoryFunctionLibrary::ResetCursorStack()
{
	ICursoryModule::Get().ResetCursorStack();
}

void UCursoryFunctionLibrary::ConformSWidgetToCursory(SWidget* Widget)
{
	if (Widget)
	{
		TAttribute<TOptional<EMouseCursor::Type>> Callback;
		Callback.BindUObject(&ICursoryModule::Get(), &UCursorySystem::GetCurrentCursorType);
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
			Callback.BindUObject(&ICursoryModule::Get(), &UCursorySystem::GetCurrentCursorType);
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
	ICursoryModule::Get().SetAutoFocusViewport(true);
}

void UCursoryFunctionLibrary::ResumeAutoFocusViewport()
{
	ICursoryModule::Get().SetAutoFocusViewport(false);
}
