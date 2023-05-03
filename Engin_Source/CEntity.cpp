#include "CEntity.h"



Entity::Entity()
	: mName(L"")
	, mID((UINT32)this)
{

}

Entity::~Entity()
{

}


namespace gui
{
	Entity::Entity()
		: mName("")
		, mID((UINT32)this)
	{

	}

	Entity::~Entity()
	{

	}
}