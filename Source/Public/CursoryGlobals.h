// © 2021 Mustafa Moiz. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DeveloperSettings.h"
#include "CursoryTypes.h"
#include "CursoryGlobals.generated.h"

class APlayerController;
class AGameModeBase;

/**********************************************************************************
*	`FCursorInfo
*	-------------
*	Information describing a custom hardware cursor.
***********************************************************************************/
USTRUCT(BlueprintType)
struct FCursorInfo 
{
	GENERATED_BODY()

public:

	/** The identifying tag for this cursor. */
	UPROPERTY(EditAnywhere, Category = Info)
	FGameplayTag Identifier;

	/** 
	 * The path to this cursor (relative to the game's Content directory). 
	 * For example, Slate/Cursors/coolcursor. Leave out file extensions. 
	 */
	UPROPERTY(EditAnywhere, Category = Info)
	FString Path;

	/** 
	 * A coordinate representing the operative point of a cursor, relative to the top-left.
	 * Coordinates should be normalized (between 0..1). For example, a typical crosshair would have a Hotspot of (0.5, 0.5). 
	 * Some cursor file types (.ani, .cur) have hotspots encoded. In this case, the file hotspot will control.
	 * This value will only be used for other image types that do not provide hotspots (.tiff, .png).
	 */
	UPROPERTY(EditAnywhere, Category = Info)
	FVector2D Hotspot;

	/** Allows using this struct as a TSet/TMap key. */
	bool operator==(const FCursorInfo& Other) const
	{
		return Identifier == Other.Identifier;
	}
};

/** Allows using this struct as a TSet/TMap key. */
FORCEINLINE uint32 GetTypeHash(const FCursorInfo& InInfo)
{
	return GetTypeHash(InInfo.Identifier);
}

/** Allows using this struct as a TSet/TMap key in Blueprints. */
template<>
struct TStructOpsTypeTraits<FCursorInfo> : public TStructOpsTypeTraitsBase2<FCursorInfo>
{
	enum
	{
		WithIdenticalViaEquality = true
	};
};

/**********************************************************************************
*	`UCursoryGlobals
*	-------------
*	Main interface for manipulating hardware cursors.
*	Loads cursors on Engine init and allows user to
*	use cursors at runtime. Loaded cursors are handled by the underlying platform, 
*	and do not need to be unloaded or deleted manually.
*	
*	Note: The "config=..." value specifies the Project Settings category.
*	The "DisplayName=..." value specifies the Project Settings section name.
***********************************************************************************/
UCLASS(config=Game, defaultconfig, meta=(DisplayName="Cursors", ToolTip="Custom hardware cursors for your game."))
class CURSORY_API UCursoryGlobals : public UDeveloperSettings
{
	GENERATED_BODY()

public:

	UCursoryGlobals(const FObjectInitializer& ObjectInitializer);

	/************************************************************************
	*
	*	``Initialization
	*
	************************************************************************/

	/** 
	 * Initializes the globals object,
	 * listening for Engine events to start loading cursors. 
	 */
	void Init();

	/************************************************************************
	*
	*	``Cursor Type
	*
	************************************************************************/
	
public:

	/** Get the current cursor type. */
	TOptional<EMouseCursor::Type> GetCurrentCursorType() const;

	DECLARE_EVENT_TwoParams(UCursoryGlobals, FCursorChanged, EMouseCursor::Type /* Cursor */, EMouseCursor::Type /* OldCursor */);

	/** Delegate for when cursor type changes. */
	FCursorChanged CursorTypeChanged;

	/************************************************************************
	*
	*	``Custom Cursor
	*
	************************************************************************/

public:

	/** Get the current custom cursor identifier. */
	FGameplayTag GetCurrentCustomCursorIdentifier() const;

private:

	/** 
	 * Loads all specified cursors on Engine startup. 
	 * Load priority:
	 * - Windows: .ani -> .cur -> .png
	 * - Mac: .tiff -> .png 
	 * - Linux: .png 
	 */
	void LoadCustomCursors();

public:

	/** Gets the number of loaded custom cursors. */
	int32 GetCustomCursorCount() const;

	/** Gets the full list of cursor options. */
	FGameplayTagContainer GetCustomCursorOptions() const;

	/** 
	 * Mounts the specified cursor for the platform's MouseCursor::Custom. 
	 * Cursor must be set to Custom to see the effect. 
	 */
	void MountCustomCursor(FGameplayTag& Identifier, bool bWidget = false);

private:

	/** Custom cursor specs. Will be loaded on Engine startup. */
	UPROPERTY(EditAnywhere, config, Category = "Cursors")
	TSet<FCursorInfo> CustomCursorSpecs;

	/** Loaded custom cursors. */
	TMap<FGameplayTag, void*> LoadedCustomCursors;

public:

	/************************************************************************
	*
	*	``Cursor Stack
	*
	************************************************************************/

private:

	/** Pushes the base cursor onto the stack. */
	void PushBaseCursor();

public:

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

private:

	/** Evaluates cursor stack. */
	void EvaluateCursorStack();

	/** Clear cursor stack (all elements). */
	void ClearCursorStack();

private:

	/** Cursor stack - topmost element dictates current cursor. */
	UPROPERTY()
	TArray<FCursorStackElement> CursorStack;

	/** Cached cursor type. */
	UPROPERTY()
	TEnumAsByte<EMouseCursor::Type> CachedCursorType;

	/** Cached custom cursor identifier. */
	UPROPERTY()
	FGameplayTag CachedCustomCursorIdentifier;

	/************************************************************************
	*
	*	``Viewport Focus
	*
	************************************************************************/

public:

	/** Set auto focus viewport. */
	void SetAutoFocusViewport(bool bActive);

private:

	/** Monitor viewport status. */
	void MonitorViewportStatus();

	/**
	 * Check viewport status (i.e. whether it is hovered, focused, etc.)
	 * to determine if we need to give focus to viewport or revert cursor
	 * to the one tied to the player.
	 * This is necessary to handle widgets properly.
	 */
	void AuditViewportStatus(float DeltaSeconds);

private:

	/**
	 * If true, automatically focuses viewport when directly hovered.
	 * Prevents reversion to default cursor when viewport loses focus (e.g. on button press).
	 * If false, users will need to manually restore focus to viewport
	 * once lost.
	 * Can be toggled at runtime.
	 */
	UPROPERTY(EditAnywhere, config, Category = "Focus")
	bool bAutoFocusViewport;
};