#include "CObjectManager.h"

ObjectManager::ObjectManager()
{
	for (UINT i = 0; i < (UINT)eWallType::End; ++i)
	{
		ObjectOffsetData.emplace_back(Vector2::Zero);
	}

	for (UINT i = 0; i < (UINT)eWallType::End; ++i)
	{
		ObjectSizeData.emplace_back(Vector2(600.f, 600.f));
	}
}

ObjectManager::~ObjectManager()
{
	while (!SkilContainer.empty())
	{
		GameObject* obj = SkilContainer.front();
		SkilContainer.pop();

		if (obj == nullptr)
			continue;

		delete obj;
	}
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

void ObjectManager::SetSizeData(eWallType type, Vector2 size)
{
	if (type == eWallType::End)
		return;

	ObjectSizeData[(UINT)type] = size;
}

Vector2 ObjectManager::GetSizeData(eWallType type)
{
	if (type == eWallType::End)
		return Vector2(100.f,100.f);

	return ObjectSizeData[(UINT)type];
}
