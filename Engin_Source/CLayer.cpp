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

void Layer::Sort_TileObject(vector<GameObject*>& object, int left, int right)
{
	if (mGameObjects.size() <= 1)
		return;

	int pivot = mGameObjects.size() / 2;
	int L = left;
	int R = right;
	GameObject* temp = 0;

	float fpivot = mGameObjects[pivot]->GetComponent<Transform>()->GetPosition().y;

	do
	{
		while (mGameObjects[L]->GetComponent<Transform>()->GetPosition().y < fpivot)
			L++;
		while (mGameObjects[L]->GetComponent<Transform>()->GetPosition().y > fpivot)
			R--;

		if (L <= R)
		{
			temp = mGameObjects[L];
			mGameObjects[L] = mGameObjects[R];
			mGameObjects[R] = temp;

			L++;
			R--;
		}

	} while (L <= R);

	if(left < R)
		Sort_TileObject(object, left, R);
	if(L < right)
		Sort_TileObject(object, L, right);

}