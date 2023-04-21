#pragma once
#include "CButton.h"

using namespace std;

class InventoryButton : public Button
{
public:
	InventoryButton();
	virtual ~InventoryButton();

	virtual void Initalize() override;
	virtual void Update() override;
	virtual void FixedUpdate() override;
	virtual void Render() override;

	virtual void Click() override;

	bool CheckPoekySlot(int& x, int& y);
	void DropItem(class ItemBase* item);

	void AddItem(ItemBase* item);
	bool DeleteItem(ItemBase* item);

	bool GetDrop() { return mbDrop; }
	void SetDrop(bool drop) { mbDrop = drop; }

	void ClearPocketSlot(ItemBase* item);

	Vector2 GetIndex() { return Vector2(mXIndex, mYIndex); }

private:
	vector<vector<int>> mPoketSlot;
	vector<ItemBase*> mPoketItem;

	bool mbDrop;

	int mXIndex;
	int mYIndex;
};

