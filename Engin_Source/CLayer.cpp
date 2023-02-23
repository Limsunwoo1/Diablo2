#include "CLayer.h"

Layer::Layer()
{
}

Layer::~Layer()
{
	for (GameObject* obj : mGameObjects)
	{
		if (obj == nullptr)
			continue;
		delete obj;
		obj = nullptr;
	}
}

void Layer::Initalize()
{
	for (GameObject* obj: mGameObjects)
	{
		if (obj == nullptr)
			continue;
		if (obj->GetState() != GameObject::active)
			continue;

		obj->Initalize();
	}
}

void Layer::Update()
{
	for (GameObject* obj : mGameObjects)
	{
		if (obj == nullptr)
			continue;
		if (obj->GetState() != GameObject::active)
			continue;

		obj->Update();
	}
}

void Layer::FixedUpdate()
{
	for (GameObject* obj : mGameObjects)
	{
		if (obj == nullptr)
			continue;
		if (obj->GetState() != GameObject::active)
			continue;

		obj->FixedUpdate();
	}
}

void Layer::Render()
{
	for (GameObject* obj : mGameObjects)
	{
		if (obj == nullptr)
			continue;
		if (obj->GetState() != GameObject::active)
			continue;

		obj->Render();
	}
}

void Layer::AddGameObject(GameObject* gameObject)
{
	if (gameObject == nullptr)
		return;

	mGameObjects.push_back(gameObject);
}
