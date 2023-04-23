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

	// 자신의 타입을 나타낸다
	eEquipmentType GetItemType() { return mType; }
	void SetItemType(eEquipmentType type) { mType;}

	// 인벤토리 배열에 leftTop 기준 인덱스를 나타낸다
	Math::Vector2 GetItemSlotSize() { return Vector2{ (float)mXSize, (float)mYSize }; }
	void SetItemSlotSize(Math::Vector2 size) { mXSize = (UINT)size.x, mYSize = (UINT)size.y; }

	void SetItemMater(Material* mater) { mMaterial = mater; }
	Material* GetItemMater() { return mMaterial; }

	const wstring& GetItemName() { return mItemName; }
	void SetItemName(const wstring& name) { mItemName = name; }

	// 인벤토리 내부 or 월드에 떨어져있는 아이템인지 판별
	bool GetStage() { return mbStage; }
	void SetStage(bool stage) { mbStage = stage; }

	// 아이템을 조건에 의해 인벤토리에 넣을수 있는지 체크
	bool GetDrop() { return mbDrop; }
	void SetDrop(bool drop) { mbDrop = drop; }

	// 클릭으로 아이템을 집었는지 체크
	bool GetPick() { return mbPick; }
	void SetPick(bool pick) { mbPick = pick; }

	// 인벤토리 내부에서 아이템이 인벤토리와 충돌하게되면
	// 상황에따라 캔버스 색을 바꿔나타낸다
	bool GetOnInventory() { return mbOnInvnetory; }
	void SetOnInventory(bool inven) { mbOnInvnetory = inven; }

	// 내가 들어가 있는 인벤토리
	void SetInventory(InventoryButton* inven) {	mInventory = inven; }
	InventoryButton* GetInventory() { return mInventory; }

	// 내가 들어가있는 아이템 슬롯
	void SetSlotInventory(InventoryButton* inven) { mSlotInventory = inven; }
	InventoryButton* GetSlotInventory() { return mSlotInventory; }

	// 나의 인벤토리 좌표
	Vector2 GetIndex() { return Vector2((float)mXIndex, (float)mYIndex); }
	void SetIndex(int& x, int& y) { mXIndex = x, mYIndex = y; }
private:
	UINT mXSize;
	UINT mYSize;

	UINT mXIndex;
	UINT mYIndex;

	Material* mMaterial;
	InventoryButton* mInventory;
	InventoryButton* mSlotInventory;

	wstring mItemName;

	eEquipmentType mType;
	bool mbStage;  // 0 world  1 poket
	bool mbPick;
	bool mbDrop;
	bool mbOnInvnetory;
};

