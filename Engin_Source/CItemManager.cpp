#include "CItemManager.h"
#include "CWorldManager.h"
#include "CInput.h"
#include "CRenderer.h"
#include "CCamera.h"

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
	mbPickUpItem = false;

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

void ItemManager::WorldRender()
{
	Matrix& view = Renderer::mainCamera->GetViewMatrix();
	Matrix& proejction = Renderer::mainCamera->GetProjectionMatrix();
	Renderer::mainCamera->SetGpuViewMatrix(view);
	Renderer::mainCamera->SetGpuProjectionMatrix(proejction);

	for (ItemBase* item : mItemes)
	{
		if (item == nullptr)
			continue;

		bool stage = item->GetStage();
		if (!stage)
			continue;

		item->Render();
	}
}

void ItemManager::Render()
{
	Matrix& view = Renderer::UiCamera->GetViewMatrix();
	Matrix& proejction = Renderer::UiCamera->GetProjectionMatrix();
	Renderer::UiCamera->SetGpuViewMatrix(view);
	Renderer::UiCamera->SetGpuProjectionMatrix(proejction);

	for (ItemBase* item : mItemes)
	{
		if (item == nullptr)
			continue;

		bool stage = item->GetStage();
		if (stage)
			continue;
		
		item->Render();
	}

	ItemBase* PickItem = Input::GetInstance()->GetPickItem();
	if (PickItem)
		ItemPushTop(PickItem);

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
			iter = mItemes.erase(iter);
			break;
		}
	}
}

void ItemManager::Rlease()
{
	for (ItemBase* item : mItemes)
	{
		if (item == nullptr)
			continue;

		delete item;
		item = nullptr;
	}

	while (!mPushQueue.empty())
	{
		ItemBase* item = mPushQueue.front();
		mPushQueue.pop();

		if (item == nullptr)
			continue;

		delete item;
		item = nullptr;
	}

	while (!mRemoveQueue.empty())
	{
		ItemBase* item = mRemoveQueue.front();
		mRemoveQueue.pop();

		if (item == nullptr)
			continue;

		delete item;
		item = nullptr;
	}
}

// 씬 이동할때 월드 드랍 아이템 초기화
void ItemManager::ClearWorldItem()
{
	for (ItemBase* item : mItemes)
	{
		if (item == nullptr)
			continue;

		if (item->GetState())
		{
			DeleteItem(item);
		}
	}
}

void ItemManager::ItemPushTop(ItemBase* item)
{
	// 아이템을 집은경우 제일 위에 렌더되어서
	// 이동중 다른 아이템보다 렌더 순위가 우선된다
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

vector<ItemBase*> ItemManager::GetSaveItemData()
{
	vector<ItemBase*> outVec = {};

	for (ItemBase* item : mItemes)
	{
		if (item->GetStage())
			continue;

		outVec.emplace_back(item);
	}

	return outVec;
}

void ItemManager::InventoryRenderStop(bool render)
{
	for (ItemBase* item : mItemes)
	{
		if (item == nullptr)
			continue;

		if (item->GetStage() == false )
		{
			/*if(item->GetItemType() != eEquipmentType::HpPotion
				&& item->GetItemType() != eEquipmentType::MpPotion
				&&item->GetItemType() != eEquipmentType::Potion)
			{ 
				item->GetComponent<BaseRenderer>()->SetRenderStop(render);
			}*/

			item->GetComponent<BaseRenderer>()->SetRenderStop(render);
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
		ItemBase* item = mRemoveQueue.front();
		mRemoveQueue.pop();

		delete item;
		item = nullptr;
	}
}
