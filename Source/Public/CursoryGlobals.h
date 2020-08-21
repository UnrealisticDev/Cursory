// Copyright Mowgl33. All rights reserved.
#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DeveloperSettings.h"
#include "CursoryGlobals.generated.h"

/**********************************************************************************
*	FCursorInfo
*	-------------
*	Information describing a custom hardware cursor.
***********************************************************************************/
USTRUCT(BlueprintType)
struct FCursorInfo 
{
	GENERATED_BODY()

public:

	/** The identifying tag for this cursor. */
	UPROPERTY(EditAnywhere)
	FGameplayTag Identifier;

	/** The path to this cursor (relative to the game's Content directory). 
	 *	For example, Slate/Cursors/coolcursor. Leave out file extensions. */
	UPROPERTY(EditAnywhere)
	FString Path;

	/** A coordinate representing the operative point of a cursor, relative to the top-left.
	 *	Coordinates should be normalized (between 0..1). For example, a typical crosshair would have a Hotspot of (0.5, 0.5). 
	 *	Some cursor file types (.ani, .cur) have hotspots encoded. In this case, the file hotspot will control.
	 *	This value will only be used for other image types that do not provide hotspots (.tiff, .png). */
	UPROPERTY(EditAnywhere)
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
*	UCursoryGlobals
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
	*	:Initialization
	*
	************************************************************************/

	/** Initializes the globals,
	 *	listening for Engine init to start loading cursors. */
	void Init();

private:

	/** Loads all specified cursors on Engine startup. 
	 *	Load priority:
	 *	- Windows: .ani -> .cur -> .png
	 *	- Mac: .tiff -> .png 
	 *	- Linux: .png */
	void LoadCursors();

	/************************************************************************
	*
	*	:Manipulation
	*
	************************************************************************/

public:

	/** Gets the number of loaded custom cursors. */
	int32 GetCustomCursorCount() const;

	/** Gets the full list of cursor options. */
	FGameplayTagContainer GetCustomCursorOptions() const;

	/** Returns the identifier for the currently mounted custom cursor. */
	FGameplayTag GetMountedCustomCursor() const;

	/** Mounts the specified cursor for the platform's MouseCursor::Custom. 
	 *	Player must set their CurrentMouseCursor to Custom to see the effect. */
	void MountCustomCursor(FGameplayTag& Identifier);

private:

	/** Custom cursor specs. Will be loaded on Engine startup. */
	UPROPERTY(EditAnywhere, config, Category = "Cursors")
	TSet<FCursorInfo> CursorSpecs;

	/** Loaded custom cursors. */
	TMap<FGameplayTag, void*> LoadedCursors;

	/** Mounted custom cursor identifier. */
	UPROPERTY()
	FGameplayTag MountedCursor;
};