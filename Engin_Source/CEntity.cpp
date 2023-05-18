#include "CEntity.h"



Entity::Entity()
	: mName(L"")
	, mID((UINT32)this)
{
	SetName(std::to_wstring((UINT32)this));
}

Entity::~Entity()
{

}


namespace gui
{
	guiEntity::guiEntity()
		: mName("")
		, mID((UINT32)this)
	{
		SetName(std::to_string((UINT32)this));
	}

	guiEntity::guiEntity(const char* name)
		: mName(name)
		, mID((UINT32)this)
	{

	}

	guiEntity::~guiEntity()
	{

	}
}