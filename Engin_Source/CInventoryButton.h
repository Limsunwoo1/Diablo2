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

private:
	vector<vector<int>> mPoketSlot;
	vector<class ItemBase*> mPoketItem;

	float mXSize;
	float mYSize;
};

