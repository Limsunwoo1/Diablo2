#include "CInventoryButton.h"
#include "CShoesItem.h"
#include "CInput.h"

InventoryButton::InventoryButton()
	: Button(eUIType::Button)
	, mXSize(0.0f)
	, mYSize(0.0f)
{
	mXSize = 0.54f;
	mYSize = 0.525f;
}


InventoryButton::~InventoryButton()
{
	for (GameObject* item : mPoketItem)
	{
		if (item == nullptr)
			continue;

		delete item;
		item = nullptr;
	}
}

void InventoryButton::Initalize()
{
	// 4 X 10 Poket
	mPoketSlot.resize(4);
	for (int i = 0; i < 4; ++i)
	{
		mPoketSlot[i].resize(10);
	}

	Button::Initalize();

	// test
	{
		ShoesItem* item = new ShoesItem(L"SmileTexture");
		item->Initalize();

		mPoketItem.push_back(item);
	}
}

void InventoryButton::Update()
{
	Button::Update();

	if (GetPointToRect() > 0 && Input::GetInstance()->GetMouseItemPick() == true)
	{
		ItemBase* item = Input::GetInstance()->GetPickItem();
		if (item)
			Input::GetInstance()->SetPickItem(nullptr);

		Transform* itemTr = item->GetComponent<Transform>();
		Vector3 ItemPos = itemTr->GetPosition();
		Vector3 ItemScale = itemTr->GetScale();

		Transform* InvenTr = this->GetComponent<Transform>();
		Vector3 InvenPos = InvenTr->GetPosition();
		Vector3 InvenScale = InvenTr->GetScale();

		if (ItemPos.x - (ItemScale.x * 0.5f) < InvenPos.x - (InvenScale.x * 0.5f)
			|| ItemPos.x + (ItemScale.x * 0.5f) > InvenPos.x + (InvenScale.x * 0.5f))
		{
			// 충돌 X
			int a = 0;
			Input::GetInstance()->SetPickItem(item);
		}
		if (ItemPos.y - (ItemScale.y * 0.5f) > InvenPos.y - (InvenScale.y * 0.5f)
			|| ItemPos.y + (ItemScale.y * 0.5f) < InvenPos.y + (InvenScale.y * 0.5f))
		{
			// 충돌 X
			int a = 0;
			Input::GetInstance()->SetPickItem(item);
		}

		float indexX = (ItemPos.x - (ItemScale.x * 0.5f));
		float indexY = (ItemPos.y + (ItemScale.y * 0.5f));

		float indexXx = (InvenPos.x - (InvenPos.x * 0.5f));
		float indexYx = (InvenPos.y + (InvenPos.y * 0.5f));

		Vector2 idx{ fabs(indexX) - fabs(indexXx) , fabs(indexY) - fabs(indexYx) };
		Vector3 IndexPos = (ItemPos - (ItemScale * 0.5f)) - (InvenPos - (InvenScale * 0.5f));

		UINT idxX = idx.x / mXSize;
		UINT idxY = idx.y / mYSize;

		int slotX = item->GetItemSlotSize().x;
		int slotY = item->GetItemSlotSize().y;
		for (int i = 0; i < slotX; ++i)
		{
			for (int j = 0; j < slotY; ++j)
			{
				if (mPoketSlot[idxY + j][idxX + i] != 0)
				{
					// 드랍 불가
					int a = 0;
					//return;
				}
			}
		}

		// Slot 채우기
		for (int i = 0; i < slotX; ++i)
		{
			for (int j = 0; j < slotY; ++j)
			{
				mPoketSlot[idxY + j][idxX + i] = 1;
			}
		}

		// Item 포지션변경
		ItemPos.x = (ItemPos.x - (ItemScale.x * 0.5f)) + idxX * mXSize;
		ItemPos.y = (InvenPos.y + (InvenPos.y * 0.5f)) - idxX * mYSize;

		itemTr->SetPosition(ItemPos);
	}

	for (GameObject* item : mPoketItem)
	{
		if (item == nullptr)
			continue;

		item->Update();
	}
}

void InventoryButton::FixedUpdate()
{
	Button::FixedUpdate();

	for (GameObject* item : mPoketItem)
	{
		if (item == nullptr)
			continue;

		item->FixedUpdate();
	}
}

void InventoryButton::Render()
{
	Button::Render();

	for (GameObject* item : mPoketItem)
	{
		if (item == nullptr)
			continue;

		item->Render();
	}
}

void InventoryButton::Click()
{
}
