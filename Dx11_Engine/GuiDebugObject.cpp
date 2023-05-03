#include "GuiDebugObject.h"


	DebugObject::DebugObject()
		: GameObject()
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
