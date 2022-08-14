// © 2021 Mustafa Moiz. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "GameplayTagContainer.h"
#include "CursoryTypes.h"
#include "CursorySystem.generated.h"

class APlayerController;
class AGameModeBase;

/**
 * Main interface for manipulating hardware cursors.
 * Loads cursors on Engine init and allows user to
 * use cursors at runtime. Loaded cursors are handled by the underlying platform, 
 * and do not need to be unloaded or deleted manually.
 */
UCLASS()
class CURSORY_API UCursorySystem : public UObject
{
	GENERATED_BODY()

public:

	/** 
	 * Initializes the globals object,
	 * listening for Engine events to start loading cursors. 
	 */
	void Init();
	
	/** Get the current cursor type. */
	TOptional<EMouseCursor::Type> GetCurrentCursorType() const;

	/** Get the current custom cursor identifier. */
	FGameplayTag GetCurrentCustomCursorIdentifier() const;

	/** Gets the number of loaded custom cursors. */
	int32 GetCustomCursorCount() const;

	/** Gets the full list of cursor options. */
	FGameplayTagContainer GetCustomCursorOptions() const;

	/** 
	 * Mounts the specified cursor for the platform's MouseCursor::Custom. 
	 * Cursor must be set to Custom to see the effect. 
	 */
	void MountCustomCursor(FGameplayTag& Identifier, bool bWidget = false);

	/** Set base cursor. */
	void ModifyBaseCursor(const FCursorStackElement& Cursor, bool bIgnoreType = false, bool bIgnoreCustom = false);

	/** Push a cursor onto the stack. */
	FCursorStackElementHandle PushCursor(FCursorStackElement Cursor);

	/** Modify a cursor on the stack by handle. */
	void ModifyCursorByHandle(FCursorStackElementHandle Handle, FCursorStackElement NewCursor);

	/** Remove a cursor from the stack by handle. */
	void RemoveCursorByHandle(FCursorStackElementHandle Handle);

	/** Pop a cursor from the stack. */
	void PopCursor();

	/** Reset cursor stack (clear all stack elements except base). */
	void ResetCursorStack();

	/** Set auto focus viewport. */
	void SetAutoFocusViewport(bool bActive);

	DECLARE_EVENT_TwoParams(UCursorySystem, FCursorChanged, EMouseCursor::Type /* Cursor */, EMouseCursor::Type /* OldCursor */);

	/** Delegate for when cursor type changes. */
	FCursorChanged CursorTypeChanged;

private:

	/** 
	 * Loads all specified cursors on Engine startup. 
	 * Load priority:
	 * - Windows: .ani -> .cur -> .png
	 * - Mac: .tiff -> .png 
	 * - Linux: .png 
	 */
	void LoadCustomCursors();

	/** Pushes the base cursor onto the stack. */
	void PushBaseCursor();

	/** Evaluates cursor stack. */
	void EvaluateCursorStack();

	/** Clear cursor stack (all elements). */
	void ClearCursorStack();
	
	/** Monitor viewport status. */
	void MonitorViewportStatus();

	/**
	 * Check viewport status (i.e. whether it is hovered, focused, etc.)
	 * to determine if we need to give focus to viewport or revert cursor
	 * to the one tied to the player.
	 * This is necessary to handle widgets properly.
	 */
	void AuditViewportStatus(float DeltaSeconds);

	/** Loaded custom cursors. */
	TMap<FGameplayTag, void*> LoadedCustomCursors;
	
	/** Cursor stack - topmost element dictates current cursor. */
	UPROPERTY()
	TArray<FCursorStackElement> CursorStack;

	/** Cached cursor type. */
	UPROPERTY()
	TEnumAsByte<EMouseCursor::Type> CachedCursorType;

	/** Cached custom cursor identifier. */
	UPROPERTY()
	FGameplayTag CachedCustomCursorIdentifier;

	TOptional<bool> bAutoFocusViewport;
};