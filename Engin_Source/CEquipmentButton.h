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

	eEquipmentType GetEquipmentType() { return mType; }
	void SetEquipmentType(eEquipmentType type) { mType = type; }

private:
	eEquipmentType mType;
	bool mbDrop;
	bool mbOnRender;
};