#include "CScene.h"

Scene::Scene()
{
	mLayers.resize((UINT)eLayerType::End);
}

Scene::~Scene()
{
}

void Scene::Initalize()
{
	for (Layer& layer : mLayers)
	{
		layer.Initalize();
	}
}

void Scene::Update()
{
	for (Layer& layer : mLayers)
	{
		layer.Update();
	}
}

void Scene::FixedUpdate()
{
	for (Layer& layer : mLayers)
	{
		layer.FixedUpdate();
	}
}

void Scene::Render()
{
	for (Layer& layer : mLayers)
	{
		layer.Render();
	}
}

void Scene::AddGameObject(GameObject* gameObj, const eLayerType type)
{
	mLayers[(UINT)type].AddGameObject(gameObj);
}
