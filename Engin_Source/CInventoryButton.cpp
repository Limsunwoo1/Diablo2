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

	mPoketSlot[0][0] = 1;
	mPoketSlot[0][1] = 1;

	mPoketSlot[1][0] = 1;
	mPoketSlot[1][1] = 1;
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

	if (/*GetPointToRect() > 0 && */Input::GetInstance()->GetMouseItemPick() == true)
	{
		ItemBase* item = Input::GetInstance()->GetPickItem();

		if (item == nullptr)
			return;

		Transform* itemTr = item->GetComponent<Transform>();
		Vector3 itemPos = itemTr->GetPosition();
		Vector3 itemScale = itemTr->GetScale();

		Transform* InvenTr = this->GetComponent<Transform>();
		Vector3 InvenPos = InvenTr->GetPosition();
		Vector3 InvenScale = InvenTr->GetScale();

		if (InvenPos.x - (InvenScale.x * 0.5f) <= itemPos.x + (itemScale.x * 0.5f)
			&& InvenPos.x + (InvenScale.x * 0.5f) >= itemPos.x - (itemScale.x * 0.5f)
			&& InvenPos.y - (InvenScale.y * 0.5f) <= itemPos.y + (itemScale.y * 0.5f)
			&& InvenPos.y + (InvenScale.y * 0.5f) >= itemPos.y - (itemScale.y * 0.5f))
		{
			SetPointToRect(1);
		}

		// 아이템 드랍 가능한지 여부
		if (InvenPos.x - (InvenScale.x * 0.5f) <= itemPos.x - (itemScale.x * 0.5f)
			&& InvenPos.x + (InvenScale.x * 0.5f) >= itemPos.x - (itemScale.x * 0.5f)
			&& InvenPos.y - (InvenScale.y * 0.5f) <= itemPos.y + (itemScale.y * 0.5f)
			&& InvenPos.y + (InvenScale.y * 0.5f) >= itemPos.y + (itemScale.y * 0.5f))
		{
			float indexX = (itemPos.x - (itemScale.x * 0.5f));
			float indexY = itemPos.y + (itemScale.y * 0.5f);

			float indexXx = (InvenPos.x - (InvenScale.x * 0.5f));
			float indexYx = (InvenPos.y + (InvenScale.y * 0.5f));

			Vector2 idx{ indexX - indexXx , indexYx - indexY };

			// X 인덱스
			int X = idx.x / mXSize;

			// Y 인덱스
			int Y = -1;
			if (indexY <= indexYx - (mYSize * 0) && indexY > indexYx - (mYSize * 1))
			{
				Y = 0;
			}
			else if (indexY <= indexYx - (mYSize * 1) && indexY > indexYx - (mYSize * 2))
			{
				Y = 1;
			}
			else if (indexY <= indexYx - (mYSize * 2) && indexY > indexYx - (mYSize * 3))
			{
				Y = 2;
			}
			else
			{
				Y = 3;
			}

			bool drop = CheckPoekySlot(X, Y);

			if (drop)
			{
				Vector2 SlotNum = item->GetItemSlotSize();
				int SlotX = (int)SlotNum.x;
				int SlotY = (int)SlotNum.y;

				for (int i = 0; i < SlotX; ++i)
				{
					for (int j = 0; j < SlotY; ++j)
					{
						if (Y + j >= mPoketSlot.size())
							continue;

						if (X + i >= mPoketSlot[Y + j].size())
							continue;

						mPoketSlot[Y + j][X + i] = 1;

					}
				}

				itemPos.x = indexXx + (mXSize * X);
				itemPos.y = indexYx - (mYSize * Y);

				itemTr->SetPosition(itemPos);

				Input::GetInstance()->SetMouseItemPick(false);
				Input::GetInstance()->SetPickItem(nullptr);
			}
			else
			{
				
			}
		}

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

bool InventoryButton::CheckPoekySlot(int& x, int& y)
{
	ItemBase* item = Input::GetInstance()->GetPickItem();

	if (item == nullptr)
		return false;

	Vector2 SlotNum = item->GetItemSlotSize();
	int SlotX = (int)SlotNum.x;
	int SlotY = (int)SlotNum.y;

	for (int i = 0; i < SlotX; ++i)
	{
		for (int j = 0; j < SlotY; ++j)
		{
			if (y + j >= mPoketSlot.size())
				continue;

			if (x + i >= mPoketSlot[y + j].size())
				continue;

			if (mPoketSlot[y + j][x + i] == 1)
			{
				return false;
			}
		}
	}
	return true;
}
