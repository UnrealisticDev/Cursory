// © 2021 Mustafa Moiz. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GenericPlatform/ICursor.h"
#include "GameplayTagContainer.h"
#include "CursoryTypes.generated.h"

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

	FCursorStackElement() {}
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
