// © 2021 Mustafa Moiz. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GenericPlatform/ICursor.h"
#include "CursoryConformerComponent.generated.h"

/**
 * Add this to a PlayerController to conform its cursor to the one 
 * dictated by the Cursory system. This allows you to change cursors
 * in one central location (Cursory System), instead of in multiple 
 * locations.
 */
UCLASS(meta=(BlueprintSpawnableComponent = true))
class UCursoryConformerComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	UCursoryConformerComponent();
	void Activate(bool bReset) override;
	void Deactivate() override;

private:

	void ConformInitialCursorState();
	void ListenForCursorChanges();
	void StopListeningForCursorChanges();
	void OnCursorChange(EMouseCursor::Type Cursor, EMouseCursor::Type OldCursor);

	FDelegateHandle CursorChangeCallbackHandle;
};
