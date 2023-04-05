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
