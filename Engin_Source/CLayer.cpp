#include "CLayer.h"

Layer::Layer()
{
}

Layer::~Layer()
{
}

void Layer::Initalize()
{
	for (GameObject* obj: mGameObjects)
	{
		if (obj == nullptr)
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

		obj->Update();
	}
}

void Layer::FixedUpdate()
{
	for (GameObject* obj : mGameObjects)
	{
		if (obj == nullptr)
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

		obj->Render();
	}
}

void Layer::AddGameObject(GameObject* gameObject)
{
	if (gameObject == nullptr)
		return;

	mGameObjects.push_back(gameObject);
}
