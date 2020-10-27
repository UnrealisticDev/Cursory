#pragma once

#include "CoreMinimal.h"
#include "GenericPlatform/ICursor.h"
#include "GameplayTagContainer.h"
#include "CursoryTypes.generated.h"

/**********************************************************************************
*	`FCursorStackElementHandle
*	-------------
*	Handle for a cursor stack element.
***********************************************************************************/
USTRUCT(BlueprintType)
struct FCursorStackElementHandle
{
	GENERATED_BODY()

public:

	FCursorStackElementHandle();

	/** Generates a unique handle. */
	static FCursorStackElementHandle Generate();

	/** Returns true if handle is valid. */
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

	/** Unique id for this handle. */
	UPROPERTY()
	int32 Id;
};

/**********************************************************************************
*	`FCursorStackElement
*	-------------
*	An element in a cursor stack.
***********************************************************************************/
USTRUCT()
struct FCursorStackElement
{
	GENERATED_BODY()

public:

	FCursorStackElement();
	FCursorStackElement(FCursorStackElementHandle InHandle);

	/** Cursor type. */
	UPROPERTY()
	TEnumAsByte<EMouseCursor::Type> CursorType;

	/** Identifier for custom cursor. */
	UPROPERTY()
	FGameplayTag CustomCursorIdentifier;

	/** Get the handle for this element. */
	FCursorStackElementHandle GetHandle() const;

	/** Allows this struct to be found in an array via equality. */
	bool operator==(const FCursorStackElement& Other) const
	{
		return Handle == Other.Handle;
	}

private:

	UPROPERTY()
	FCursorStackElementHandle Handle;
};
