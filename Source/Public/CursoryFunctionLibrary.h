// Copyright Mowgl33. All rights reserved.
#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GameplayTagContainer.h"
#include "GenericPlatform/ICursor.h"
#include "CursoryFunctionLibrary.generated.h"

class UWidget;

/**********************************************************************************
*	UCursoryFunctionLibrary
*	-------------
*	Function library for easy access to Cursory functionality.
*	Preferred entry point for end users.
***********************************************************************************/
UCLASS()
class UCursoryFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	/** 
	 * Sets the hardware cursor for player one.
	 * Do not use this to set the cursor to Custom. 
	 * Note: Does not affect widget cursors.
	 */
	UFUNCTION(BlueprintCallable, Category = "Cursory", meta = (WorldContext = "WorldContextObject"))
	static void UseStandardCursor(const UObject* WorldContextObject, EMouseCursor::Type Cursor);

	/** 
	 * Set the hardware cursor for player one. 
	 * Note: Does not affect widget cursors.
	 */
	UFUNCTION(BlueprintCallable, Category = "Cursory", meta = (WorldContext = "WorldContextObject"))
	static void UseCustomCursor(const UObject* WorldContextObject, FGameplayTag Identifier);

	/** 
	 * Set the hardware cursor for the specified widget.
	 * Do not use this to set the cursor to Custom.
	 * Note: Does not affect player cursor.
	 */
	UFUNCTION(BlueprintCallable, Category = "Cursory", meta=(DefaultToSelf=Widget))
	static void UseWidgetStandardCursor(UWidget* Widget, EMouseCursor::Type Cursor);

	/**
	 * Set the hardware cursor for the specified widget.
	 * Note: Does not affect player cursor.
	 */
	UFUNCTION(BlueprintCallable, Category = "Cursory", meta=(DefaultToSelf=Widget))
	static void UseWidgetCustomCursor(UWidget* Widget, FGameplayTag Identifier);

	/** 
	 * Set the hardware cursor for the specified widget to 
	 * the player cursor. 
	 */
	UFUNCTION(BlueprintCallable, Category = "Cursory", meta=(DefaultToSelf=Widget, WorldContext = "Widget"))
	static void UsePlayerCursorForWidget(UWidget* Widget);

	/** Pause automatically focusing viewport when directly hovered. */
	UFUNCTION(BlueprintCallable, Category = "Cursory")
	static void PauseAutoFocusViewport();

	/** Resume automatically focusing viewport when directly hovered. */
	UFUNCTION(BlueprintCallable, Category = "Cursory")
	static void ResumeAutoFocusViewport();
};