#include "CObjectManager.h"

ObjectManager::ObjectManager()
{

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

SkilFire* ObjectManager::GetSkilObj(Player* owner)
{

	SkilFire* out = nullptr;

	if (SkilContainer.empty())
	{
		out = new SkilFire(owner);
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
