#include "CLayer.h"
#include "CRenderer.h"
#include "CTransform.h"
#include "CObjectManager.h"

Layer::Layer()
{
}

Layer::~Layer()
{
	for (GameObject* obj : mGameObjects)
	{
		if (obj == nullptr)
			continue;
		if (obj->GetLayerType() == eLayerType::UI)
			continue;

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