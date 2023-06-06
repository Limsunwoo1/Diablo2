#include "CLayer.h"
#include "CRenderer.h"
#include "CTransform.h"
#include "CObjectManager.h"
#include "CWorldManager.h"

Layer::Layer()
{
	mbSortTile = false;
}

Layer::~Layer()
{
	for (GameObject* obj : mGameObjects)
	{
		if (obj == nullptr)
			continue;
		if (obj->GetLayerType() == eLayerType::UI)
			continue;

		if (obj->GetLayerType() == eLayerType::Player)
		{
			WorldManager::GetInstance()->SetPlayer(nullptr);
		}

		delete obj;
		obj = nullptr;
	}
}

void Layer::Initalize()
{
	
}

void Layer::Update()
{
	for (GameObject* obj : mGameObjects)
	{
		if (obj == nullptr)
			continue;
		if (obj->GetState() != GameObject::active)
			continue;

		if (obj->GetLayerType() == eLayerType::Tile)
		{
		

		}


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

void Layer::Destroy()
{
	// 탐색속도를위해 트리형식에 std::set<> 사용
	std::set<GameObject*> deleteObjects;
	// 삭제할 오브젝트를 전부 찾아온다
	for (GameObject* gameObj : mGameObjects)
	{
		if (gameObj->GetState() == GameObject::dead)
		{
			if (gameObj->GetLayerType() == eLayerType::UI)
				continue;

			deleteObjects.insert(gameObj);
		}
	}

	// 지워야할 오브젝트들 게임 오브젝트 모음안에서 삭제
	for (GameObjectIter iter = mGameObjects.begin(); iter != mGameObjects.end();)
	{
		std::set<GameObject*>::iterator deleteIter = deleteObjects.find(*iter);

		if (deleteIter != deleteObjects.end())
			iter = mGameObjects.erase(iter);
		else
			++iter;
	}
	
	// 삭제할 오브젝트들을 실제 메모리 할당 해제
	for (GameObject* gameObj : deleteObjects)
	{
		delete gameObj;
		gameObj = nullptr;
	}
}

void Layer::AddGameObject(GameObject* gameObject)
{
	if (gameObject == nullptr)
		return;

	mGameObjects.push_back(gameObject);
}

std::vector<GameObject*> Layer::GetDontDestroyGameObjects()
{
	std::vector<GameObject*> donts;
	for (GameObjectIter iter = mGameObjects.begin(); iter != mGameObjects.end();)
	{
		if ((*iter)->IsDontDestroty() == true)
		{
			donts.push_back((*iter));
			iter = mGameObjects.erase(iter);
		}
		else
		{
			iter++;
		}
	}

	return donts;
}