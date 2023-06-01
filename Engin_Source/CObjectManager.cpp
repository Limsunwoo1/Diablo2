#include "CObjectManager.h"

ObjectManager::ObjectManager()
{
	for (UINT i = 0; i < (UINT)eWallType::End; ++i)
	{
		ObjectOffsetData.emplace_back(Vector2::Zero);
	}
}

ObjectManager::~ObjectManager()
{

}

void ObjectManager::Update()
{
}

void ObjectManager::FixedUpdate()
{
}

void ObjectManager::Initialize()
{
}

Skil* ObjectManager::GetSkilObj(Player* owner)
{
	Skil* out = nullptr;

	if (SkilContainer.empty())
	{
		out = new Skil(owner);
	}
	else
	{
		out = SkilContainer.front();
		SkilContainer.pop();

		out->Active();

		out->SetOwner(owner);
	}

	return out;
}

void ObjectManager::SetOffsetData(eWallType type, Vector2 offset)
{
	if (type == eWallType::End)
		return;

	ObjectOffsetData[(UINT)type] = offset;
}

Vector2 ObjectManager::GetOffsetData(eWallType type)
{
	if (type == eWallType::End)
		return Vector2::Zero;

	return ObjectOffsetData[(UINT)type];
}
