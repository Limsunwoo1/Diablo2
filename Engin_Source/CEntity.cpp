#include "CEntity.h"



Entity::Entity()
	: mName(L"")
	, mID((UINT32)this)
{
	SetName(std::to_wstring(mID));
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
		SetName(std::to_string((UINT32)this));
	}

	Entity::Entity(const char* name)
		: mName(name)
		, mID((UINT32)this)
	{

	}

	Entity::~Entity()
	{

	}
}