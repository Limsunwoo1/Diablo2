#pragma once
#include "CGameObject.h"
#include "CMaterial.h"
#include "CInventoryButton.h"


class ItemBase : public GameObject
{
public:
	ItemBase(eEquipmentType type);
	virtual ~ItemBase();

	virtual void Initalize() override {};
	virtual void Update() override;
	virtual void FixedUpdate() override;
	virtual void Render() override;

	virtual void InitAnimation() override {};

	eEquipmentType GetItemType() { return mType; }
	void SetItemType(eEquipmentType type) { mType;}

	Math::Vector2 GetItemSlotSize() { return Vector2{ (float)mXSize, (float)mYSize }; }
	void SetItemSlotSize(Math::Vector2 size) { mXSize = (UINT)size.x, mYSize = (UINT)size.y; }

	void SetItemMater(Material* mater) { mMaterial = mater; }
	Material* GetItemMater() { return mMaterial; }

	const wstring& GetItemName() { return mItemName; }
	void SetItemName(const wstring& name) { mItemName = name; }

	bool GetStage() { return mbStage; }
	void SetStage(bool stage) { mbStage = stage; }

	void SetInventory(InventoryButton* inven) {	mInventory = inven; }
	InventoryButton* GetInventory() { return mInventory; }

	Vector2 GetIndex() { return Vector2((float)mXIndex, (float)mYIndex); }
	void SetIndex(int& x, int& y) { mXIndex = x, mYIndex = y; }
private:
	UINT mXSize;
	UINT mYSize;

	UINT mXIndex;
	UINT mYIndex;

	Material* mMaterial;
	class InventoryButton* mInventory;

	wstring mItemName;

	eEquipmentType mType;
	bool mbStage;  // 0 world  1 poket
	bool mbPick;
	bool mbDrop;
};

