#include "CLayer.h"
#include "CRenderer.h"
#include "CTransform.h"
#include "CObjectManager.h"
#include "CWorldManager.h"
#include "CSceneManager.h"


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
	if (SceneManager::GetInstance()->GetActiveScene()->GetScenType() != eSceneType::Tool)
	{
		if (mType == eLayerType::TileCarve)
			return;
	}

	if (mType == eLayerType::Tile || mType == eLayerType::Wall)
	{
		if (mType == eLayerType::Tile)
		{
			std::vector<GameObject*> objects = ObjectManager::GetInstance()->GetTileRenderObject();

			for (GameObject* obj : objects)
			{
				if (obj == nullptr)
					continue;

				obj->Update();
			}
		}
		else if (mType == eLayerType::Wall)
		{
			std::vector<GameObject*> objects = ObjectManager::GetInstance()->GetWallRenderObejct();

			for (GameObject* obj : objects)
			{
				if (obj == nullptr)
					continue;

				obj->Update();
			}
		}

		return;
	}

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
	if (SceneManager::GetInstance()->GetActiveScene()->GetScenType() != eSceneType::Tool)
	{
		if (mType == eLayerType::TileCarve)
			return;
	}

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
	/*if (mType == eLayerType::TileCarve)
		return;*/

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
	// Ž���ӵ������� Ʈ�����Ŀ� std::set<> ���
	std::set<GameObject*> deleteObjects;
	// ������ ������Ʈ�� ���� ã�ƿ´�
	for (GameObject* gameObj : mGameObjects)
	{
		if (gameObj->GetState() == GameObject::dead)
		{
			if (gameObj->GetLayerType() == eLayerType::UI)
				continue;

			deleteObjects.insert(gameObj);
		}
	}

	// �������� ������Ʈ�� ���� ������Ʈ �����ȿ��� ����
	for (GameObjectIter iter = mGameObjects.begin(); iter != mGameObjects.end();)
	{
		std::set<GameObject*>::iterator deleteIter = deleteObjects.find(*iter);

		if (deleteIter != deleteObjects.end())
			iter = mGameObjects.erase(iter);
		else
			++iter;
	}
	
	// ������ ������Ʈ���� ���� �޸� �Ҵ� ����
	for (GameObject* gameObj : deleteObjects)
	{
		if (gameObj->GetLayerType() == eLayerType::Tile || gameObj->GetLayerType() == eLayerType::Wall)
		{
			TileObject* tile = dynamic_cast<TileObject*>(gameObj);
			if (tile != nullptr)
			{
				ObjectManager::GetInstance()->DeleteTileObject(tile);
			}

			WallObject* wall = dynamic_cast<WallObject*>(gameObj);
			if (wall != nullptr)
			{
				ObjectManager::GetInstance()->DeleteWallObjet(wall);
			}

			ObjectManager::GetInstance()->ResetWorld();
		}

		delete gameObj;
		gameObj = nullptr;
	}
}

void Layer::SortObjects()
{
	std::sort(mGameObjects.begin(), mGameObjects.end(), [](GameObject* a, GameObject* b)
		{
			Transform* aTr = a->GetComponent<Transform>();
			Transform* bTr = b->GetComponent<Transform>();

			Vector3 aPos = aTr->GetPosition() + aTr->GetOffset() - (aTr->GetSize() * 0.5f);
			Vector3 bPos = bTr->GetPosition() + bTr->GetOffset() - (bTr->GetSize() * 0.5f);

			if (aPos.y == bPos.y)
				return a < b;
			else
				return aPos.y > bPos.y;
		});
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