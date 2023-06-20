#pragma once
#include "CGameObject.h"
#include "CMaterial.h"
#include "CInventoryButton.h"
#include "CAnimator.h"


class ItemBase : public GameObject
{
public:
	struct ItemSaveData
	{
		UINT ItemType;
		UINT ItemStage;
		UINT ItemIndexX;
		UINT ItemIndexY;
	};

public:
	ItemBase(eEquipmentType type);
	virtual ~ItemBase();

	virtual void Initalize() override;
	virtual void Update() override;
	virtual void FixedUpdate() override;
	virtual void Render() override;

	virtual void InitAnimation() override {};

	// �ڽ��� Ÿ���� ��Ÿ����
	eEquipmentType GetItemType() { return mType; }
	void SetItemType(eEquipmentType type) { mType;}

	// �κ��丮 �迭�� leftTop ���� �ε����� ��Ÿ����
	Math::Vector2 GetItemSlotSize() { return Vector2{ (float)mXSize, (float)mYSize }; }
	void SetItemSlotSize(Math::Vector2 size) { mXSize = (UINT)size.x, mYSize = (UINT)size.y; }

	void SetItemMater(Material* mater) { mMaterial = mater; }
	Material* GetItemMater() { return mMaterial; }

	const wstring& GetItemName() { return mItemName; }
	void SetItemName(const wstring& name) { mItemName = name; }

	// �κ��丮 ���� or ���忡 �������ִ� ���������� �Ǻ�
	bool GetStage() { return mbStage; }
	void SetStage(bool stage) { mbStage = stage; }

	// �������� ���ǿ� ���� �κ��丮�� ������ �ִ��� üũ
	bool GetDrop() { return mbDrop; }
	void SetDrop(bool drop) { mbDrop = drop; }

	// Ŭ������ �������� �������� üũ
	bool GetPick() { return mbPick; }
	void SetPick(bool pick) { mbPick = pick; }

	// �κ��丮 ���ο��� �������� �κ��丮�� �浹�ϰԵǸ�
	// ��Ȳ������ ĵ���� ���� �ٲ㳪Ÿ����
	bool GetOnInventory() { return mbOnInvnetory; }
	void SetOnInventory(bool inven) { mbOnInvnetory = inven; }

	// ���� �� �ִ� �κ��丮
	void SetInventory(InventoryButton* inven) {	mInventory = inven; }
	InventoryButton* GetInventory() { return mInventory; }

	// ���� ���ִ� ������ ����
	void SetSlotInventory(InventoryButton* inven) { mSlotInventory = inven; }
	InventoryButton* GetSlotInventory() { return mSlotInventory; }

	// ���� �κ��丮 ��ǥ
	Vector2 GetIndex() { return Vector2((float)mXIndex, (float)mYIndex); }
	void SetIndex(int& x, int& y) { mXIndex = x, mYIndex = y; }
	void SetIndex(UINT x , UINT y) { mXIndex = x, mYIndex = y; }

	void SetTargetObject(GameObject* obj) { mTargetObject = obj; }
	GameObject* GetTargetObject() { return mTargetObject; }

	Vector3& GetWorldSize() { return mWorldScale; }
	void SetWorldSize(const Vector3& scale) { mWorldScale = scale; }

	Vector3& GetInvenSize() { return mInvenScale; }
	void SetInvenSize(const Vector3& scale) { mInvenScale = scale; }

	void Drop();
private:
	UINT mXSize;
	UINT mYSize;

	UINT mXIndex;
	UINT mYIndex;

	Vector3 mWorldScale;
	Vector3 mInvenScale;

	Material* mMaterial;
	InventoryButton* mInventory;
	InventoryButton* mSlotInventory;

	wstring mItemName;

	eEquipmentType mType;
	bool mbStage;  // 0 poket  1 world
	bool mbPick;
	bool mbDrop;
	bool mbOnInvnetory;

	GameObject* mTargetObject;
};

