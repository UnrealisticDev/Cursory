// © 2021 Mustafa Moiz. All rights reserved.

#include "CursoryTypes.h"

FCursorStackElementHandle::FCursorStackElementHandle()
	: Id(-1)
{
	// Stub
}

FCursorStackElementHandle FCursorStackElementHandle::Generate()
{
	static int32 Id = 0;

	FCursorStackElementHandle Handle;
	Handle.Id = Id;

	++Id;

	return Handle;
}

bool FCursorStackElementHandle::IsValid() const
{
	return Id != -1;
}

FCursorStackElement::FCursorStackElement()
	: CursorType(EMouseCursor::Default)
{
	// Stub
}

FCursorStackElement::FCursorStackElement(FCursorStackElementHandle InHandle)
	: CursorType(EMouseCursor::Default)
	, Handle(InHandle)
{
	// Stub
}

FCursorStackElementHandle FCursorStackElement::GetHandle() const
{
	return Handle;
}
