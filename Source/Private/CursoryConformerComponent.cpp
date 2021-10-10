// © 2021 Mustafa Moiz. All rights reserved.

#include "CursoryConformerComponent.h"
#include "GameFramework/PlayerController.h"
#include "CursoryModule.h"
#include "CursoryGlobals.h"

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
	UCursoryGlobals* Globals = ICursoryModule::Get().GetGlobals();
	APlayerController* PlayerOwner = Cast<APlayerController>(GetOwner());
	
	if (Globals && PlayerOwner)
	{
		PlayerOwner->CurrentMouseCursor = Globals->GetCurrentCursorType().GetValue();
	}
}

void UCursoryConformerComponent::ListenForCursorChanges()
{
	if (UCursoryGlobals* Globals = ICursoryModule::Get().GetGlobals())
	{
		CursorChangeCallbackHandle = Globals->CursorTypeChanged.AddUObject(this, &UCursoryConformerComponent::OnCursorChange);
	}
}

void UCursoryConformerComponent::StopListeningForCursorChanges()
{
	if (UCursoryGlobals* Globals = ICursoryModule::Get().GetGlobals())
	{
		Globals->CursorTypeChanged.Remove(CursorChangeCallbackHandle);
	}
}

void UCursoryConformerComponent::OnCursorChange(EMouseCursor::Type Cursor, EMouseCursor::Type OldCursor)
{
	if (APlayerController* PlayerOwner = Cast<APlayerController>(GetOwner()))
	{
		PlayerOwner->CurrentMouseCursor = Cursor;
	}
}
