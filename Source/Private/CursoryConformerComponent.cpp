// © 2021 Mustafa Moiz. All rights reserved.

#include "CursoryConformerComponent.h"
#include "GameFramework/PlayerController.h"
#include "CursoryModule.h"
#include "CursorySystem.h"

UCursoryConformerComponent::UCursoryConformerComponent()
{
	bAutoActivate = true;
}

void UCursoryConformerComponent::Activate(bool bReset)
{
	Super::Activate(bReset);

	ConformInitialCursorState();
	ListenForCursorChanges();
}

void UCursoryConformerComponent::Deactivate()
{
	StopListeningForCursorChanges();

	Super::Deactivate();
}

void UCursoryConformerComponent::ConformInitialCursorState()
{
	if (APlayerController* PlayerOwner = Cast<APlayerController>(GetOwner()))
	{
		PlayerOwner->CurrentMouseCursor = ICursoryModule::Get().GetCurrentCursorType().GetValue();
	}
}

void UCursoryConformerComponent::ListenForCursorChanges()
{
	CursorChangeCallbackHandle = ICursoryModule::Get().CursorTypeChanged.AddUObject(this, &UCursoryConformerComponent::OnCursorChange);
}

void UCursoryConformerComponent::StopListeningForCursorChanges()
{
	ICursoryModule::Get().CursorTypeChanged.Remove(CursorChangeCallbackHandle);
}

void UCursoryConformerComponent::OnCursorChange(EMouseCursor::Type Cursor, EMouseCursor::Type OldCursor)
{
	if (APlayerController* PlayerOwner = Cast<APlayerController>(GetOwner()))
	{
		PlayerOwner->CurrentMouseCursor = Cursor;
	}
}
