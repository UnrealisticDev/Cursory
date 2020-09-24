// Copyright Mowgl33. All rights reserved.
#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GameplayTagContainer.h"
#include "GenericPlatform/ICursor.h"
#include "CursoryFunctionLibrary.generated.h"

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
	 * Sets the first player's hardware cursor to one of the standard types.
	 * Do not use this to set the cursor to Custom. 
	 */
	UFUNCTION(BlueprintCallable, Category = "Cursory", meta = (WorldContext = "WorldContextObject"))
	static void UseStandardCursor(const UObject* WorldContextObject, EMouseCursor::Type Cursor);

	/** 
	 * Set the first player's hardware cursor to the specified
	 * custom cursor. 
	 */
	UFUNCTION(BlueprintCallable, Category = "Cursory", meta = (WorldContext = "WorldContextObject"))
	static void UseCustomCursor(const UObject* WorldContextObject, FGameplayTag Identifier);
};