#pragma once
#include "CInventoryButton.h"

class EquipmentButton : public InventoryButton
{
public:
	EquipmentButton(eEquipmentType type);
	virtual ~EquipmentButton();

	virtual void Initalize() override;
	virtual void Update() override;
	virtual void FixedUpdate() override;
	virtual void Render() override;

	virtual void Click() override;

	virtual void DropItem(class ItemBase* item) override;
	virtual void ClearPocketSlot(ItemBase* item) override;

	eEquipmentType GetEquipmentType() { return mType; }
	void SetEquipmentType(eEquipmentType type) { mType = type; }

	void SetItem(ItemBase* item) { mItem = item; }
	ItemBase* GetItem() { return mItem; }

private:
	ItemBase* mItem;
	eEquipmentType mType;
	bool mbDrop;
	bool mbOnRender;
};