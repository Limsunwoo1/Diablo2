#pragma once
#include "CEngine.h"
#include "CItemBase.h"

using namespace std;
class ItemManager
{
	SINGLE(ItemManager)
public:
	void Initalize();
	void Update();
	void FixedUpdate();
	void WorldRender();
	void Render();

	void ItemPushTop(ItemBase* item);

	void AddItem(ItemBase* item);
	void DeleteItem(ItemBase* item);

	void Rlease();
	void ClearWorldItem();

	void SetPickUpItem(bool pick) { mbPickUpItem = pick; }
	bool GetPickUpItem() { return mbPickUpItem; }

	vector<ItemBase*> GetSaveItemData();
	void PushItem(ItemBase* item) { mItemes.emplace_back(item); }

	const vector<ItemBase*>& GetItems() { return mItemes; }
private:
	void pushItem();
	void removeItem();
private:
	vector<ItemBase*> mItemes;
	queue<ItemBase*> mPushQueue;
	queue<ItemBase*> mRemoveQueue;

	bool mbPickUpItem;
};

