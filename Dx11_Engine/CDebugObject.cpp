#include "CDebugObject.h"

DebugObject::DebugObject()
{
}

DebugObject::~DebugObject()
{
}

void DebugObject::FixedUpdate()
{
	for (Component* com : mComponents)
	{
		if (com == nullptr)
			continue;

		com->FixedUpdate();
	}
}
