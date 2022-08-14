// © 2021 Mustafa Moiz. All rights reserved.

#include "CursoryTypes.h"

FCursorStackElementHandle FCursorStackElementHandle::Generate()
{
	static int32 Id{0};

	FCursorStackElementHandle Handle;
	Handle.Id = Id++;
	return Handle;
}

bool FCursorStackElementHandle::IsValid() const
{
	return Id != -1;
}

FCursorStackElement::FCursorStackElement(FCursorStackElementHandle InHandle)
	: Handle(InHandle)
{
	// no op
}

FCursorStackElementHandle FCursorStackElement::GetHandle() const
{
	return Handle;
}
