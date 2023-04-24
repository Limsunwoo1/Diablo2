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
	void Render();

	void AddItem(ItemBase* item);
	void DeleteItem(ItemBase* item);

private:
	void pushItem();
	void removeItem();
private:
	vector<ItemBase*> mItemes;
	queue<ItemBase*> mPushQueue;
	queue<ItemBase*> mRemoveQueue;
};

