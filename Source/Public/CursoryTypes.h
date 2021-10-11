// © 2021 Mustafa Moiz. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GenericPlatform/ICursor.h"
#include "GameplayTagContainer.h"
#include "CursoryTypes.generated.h"

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

USTRUCT(BlueprintType)
struct FCursorStackElementHandle
{
	GENERATED_BODY()

public:

	static FCursorStackElementHandle Generate();
	
	bool IsValid() const;

	/** Allow this struct to be used as TSet/TMap key. */
	bool operator==(const FCursorStackElementHandle& Other) const
	{
		return Id == Other.Id;
	}

	/** Allow this struct to be used as TSet/TMap key. */
	friend uint32 GetTypeHash(const FCursorStackElementHandle& Handle)
	{
		return GetTypeHash(Handle.Id);
	}

private:

	UPROPERTY()
	int32 Id{-1};
};

USTRUCT()
struct FCursorStackElement
{
	GENERATED_BODY()

public:

	FCursorStackElement() = default;
	FCursorStackElement(FCursorStackElementHandle InHandle);

	FCursorStackElementHandle GetHandle() const;

	/** Allows this struct to be found in an array via equality. */
	bool operator==(const FCursorStackElement& Other) const
	{
		return Handle == Other.Handle;
	}

	UPROPERTY()
	TEnumAsByte<EMouseCursor::Type> CursorType{EMouseCursor::Default};

	UPROPERTY()
	FGameplayTag CustomCursorIdentifier;

private:

	UPROPERTY()
	FCursorStackElementHandle Handle;
};
