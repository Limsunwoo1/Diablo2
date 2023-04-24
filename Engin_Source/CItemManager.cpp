#include "CItemManager.h"
#include "CWorldManager.h"

ItemManager::ItemManager()
{

}

ItemManager::~ItemManager()
{

}

void ItemManager::Initalize()
{

}

void ItemManager::Update()
{
	GameObject* player = WorldManager::GetInstance()->GetPlayer();
	for (ItemBase* item : mItemes)
	{
		if (item == nullptr)
			continue;

		if (player)
			item->SetTargetObject(player);

		item->Update();
	}
}

void ItemManager::FixedUpdate()
{
	for (ItemBase* item : mItemes)
	{
		if (item == nullptr)
			continue;

		item->FixedUpdate();
	}
}

void ItemManager::Render()
{
	for (ItemBase* item : mItemes)
	{
		if (item == nullptr)
			continue;

		item->Render();
	}

	removeItem();
	pushItem();
}

void ItemManager::AddItem(ItemBase* item)
{
	vector<ItemBase*>::iterator iter;
	for (iter = mItemes.begin(); iter != mItemes.end(); ++iter)
	{
		if (*iter == item)
			return;
	}

	mPushQueue.push(item);
}

void ItemManager::DeleteItem(ItemBase* item)
{
	vector<ItemBase*>::iterator iter;
	for (iter = mItemes.begin(); iter != mItemes.end(); ++iter)
	{
		if (*iter == item)
		{
			mRemoveQueue.push(item);
			mItemes.erase(iter);
			break;
		}
	}
}

void ItemManager::ItemPushTop(ItemBase* item)
{
	// �������� ������� ���� ���� �����Ǿ
	// �̵��� �ٸ� �����ۺ��� ���� ������ �켱�ȴ�
	vector<ItemBase*>::iterator iter;
	for (iter = mItemes.begin(); iter != mItemes.end(); ++iter)
	{
		if (*iter == item)
		{
			mItemes.erase(iter);
			AddItem(item);
			break;
		}
	}
}

void ItemManager::pushItem()
{
	while (!mPushQueue.empty())
	{
		ItemBase* item = mPushQueue.front();
		mPushQueue.pop();

		mItemes.push_back(item);
	}
}

void ItemManager::removeItem()
{
	while (!mRemoveQueue.empty())
	{
		ItemBase* item = mPushQueue.front();
		mPushQueue.pop();

		delete item;
		item = nullptr;
	}
}
