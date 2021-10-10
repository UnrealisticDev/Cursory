// © 2021 Mustafa Moiz. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GameplayTagContainer.h"
#include "GenericPlatform/ICursor.h"
#include "CursoryTypes.h"
#include "CursoryFunctionLibrary.generated.h"

class UWidget;
class UUserWidget;
class SWidget;

/**
 * Function library for easy access to Cursory functionality.
 * Preferred entry point for end users.
 */
UCLASS()
class UCursoryFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "Cursory", meta = (WorldContext = "WorldContextObject", DeprecatedFunction, DeprecationMessage="Use UseBaseStandardCursor() instead."))
	static void UseStandardCursor(const UObject* WorldContextObject, EMouseCursor::Type Cursor);

	UFUNCTION(BlueprintCallable, Category = "Cursory", meta = (WorldContext = "WorldContextObject", DeprecatedFunction, DeprecationMessage = "Use UseBaseCustomCursor() instead."))
	static void UseCustomCursor(const UObject* WorldContextObject, FGameplayTag Identifier);

	/** Reset the base cursor to default. */
	UFUNCTION(BlueprintCallable, Category = "Cursory")
	static void ResetBaseCursor();

	/** Set the base cursor to a standard type. */
	UFUNCTION(BlueprintCallable, Category = "Cursory")
	static void UseBaseStandardCursor(EMouseCursor::Type Cursor);

	/** Set the base cursor to a custom type. */
	UFUNCTION(BlueprintCallable, Category = "Cursory")
	static void UseBaseCustomCursor(FGameplayTag Identifier);

	/** Push a (standard) cursor onto the stack. */
	UFUNCTION(BlueprintCallable, Category = "Cursory")
	static FCursorStackElementHandle PushStandardCursor(EMouseCursor::Type Cursor);

	/** Push a (custom) cursor onto the stack. */
	UFUNCTION(BlueprintCallable, Category = "Cursory")
	static FCursorStackElementHandle PushCustomCursor(FGameplayTag Identifier);

	/** 
	 * Update a cursor on the stack to standard, by handle.
	 * Does nothing if cursor associated with handle is not on stack.
	 */
	UFUNCTION(BlueprintCallable, Category = "Cursory")
	static void SetStandardCursorByHandle(FCursorStackElementHandle Handle, EMouseCursor::Type Cursor);

	/** 
	 * Update a cursor on the stack to custom, by handle. 
	 * Does nothing if cursor associated with handle is not on stack.
	 */
	UFUNCTION(BlueprintCallable, Category = "Cursory")
	static void SetCustomCursorByHandle(FCursorStackElementHandle Handle, FGameplayTag Identifier);

	/** Remove a cursor from the stack, by handle. */
	UFUNCTION(BlueprintCallable, Category = "Cursory")
	static void RemoveCursorByHandle(FCursorStackElementHandle Handle);

	/** 
	 * Pop a cursor from the stack.
	 * Does nothing if only the base cursor is on the stack.
	 */
	UFUNCTION(BlueprintCallable, Category = "Cursory")
	static void PopCursor();

	/** Reset cursor stack (clear all stack elements except base). */
	UFUNCTION(BlueprintCallable, Category = "Cursory")
	static void ResetCursorStack();

	/** Conforms the specified SWidget to use the global Cursory cursor. */
	static void ConformSWidgetToCursory(SWidget* Widget);

	/** Conforms the specified widget to use the global Cursory cursor. */
	UFUNCTION(BlueprintCallable, Category = "Cursory")
	static void ConformWidgetToCursory(UWidget* Widget);

	/** Conforms the specified widgets to use the global Cursory cursor. */
	UFUNCTION(BlueprintCallable, Category = "Cursory")
	static void ConformWidgetsToCursory(TArray<UWidget*> Widgets);

	/** 
	 * Conforms the specified widget and all children 
	 * (and optionally descendant UserWidgets) 
	 * to use the global Cursory cursor. 
	 */
	UFUNCTION(BlueprintCallable, Category = "Cursory", meta=(AdvancedDisplay=1, DisplayName="Conform Widget to Cursory (Recursive)"))
	static void ConformWidgetToCursoryRecursive(UUserWidget* Widget, bool bDescendantUserWidgets = false);

	/** Pause automatically focusing viewport when directly hovered. */
	UFUNCTION(BlueprintCallable, Category = "Cursory")
	static void PauseAutoFocusViewport();

	/** Resume automatically focusing viewport when directly hovered. */
	UFUNCTION(BlueprintCallable, Category = "Cursory")
	static void ResumeAutoFocusViewport();
};