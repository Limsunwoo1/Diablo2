#pragma once
#include "CPanel.h"

class InventoryButton;
class EquipmentButton;
class InventoryPanel : public Panel
{
public:
	InventoryPanel();
	virtual ~InventoryPanel();

	virtual void Initalize() override;
	virtual void Update() override;
	virtual void FixedUpdate() override;
	virtual void Render() override;

	virtual void InitAnimation() override;

	virtual void OnActive() override;
	virtual void UnActive() override;

	InventoryButton* GetInventoryArr() { return mInvenArr; }
	void SetInvenArr(InventoryButton* arr) { mInvenArr = arr; }

	vector<EquipmentButton*> GetSlotButton() { return mSlotButton; }
private:
	InventoryButton* mInvenArr;
	vector<EquipmentButton*> mSlotButton;
};

