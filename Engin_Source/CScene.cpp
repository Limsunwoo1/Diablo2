#include "CScene.h"
#include "CCamera.h"
#include "CGameObject.h"

Scene::Scene(eSceneType type)
	: mType(type)
	, mMainCamera(nullptr)
{
	mLayers.resize((UINT)eLayerType::End);

	for (int i = 0;i < mLayers.size(); ++i)
	{
		mLayers[i].SetLayerType(eLayerType(i));
	}
}

Scene::~Scene()
{
}

void Scene::Initalize()
{
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

void Scene::Destroy()
{
	for (Layer& layer : mLayers)
	{
		layer.Destroy();
	}
}

void Scene::AddGameObject(GameObject* gameObj, const eLayerType type)
{
	mLayers[(UINT)type].AddGameObject(gameObj);
	gameObj->SetLayerType(type);
}

std::vector<GameObject*> Scene::GetDontDestroyObjects()
{
	std::vector<GameObject*> gameObjects;
	for (Layer& layer : mLayers)
	{
		std::vector<GameObject*> dontGameObjs = layer.GetDontDestroyGameObjects();

		gameObjects.insert(gameObjects.end(), dontGameObjs.begin(), dontGameObjs.end());
	}

	return gameObjects;
}

const std::vector<GameObject*>& Scene::GetGameObject(const eLayerType type)
{
	return mLayers[(UINT)type].GetGameObjects();
}
