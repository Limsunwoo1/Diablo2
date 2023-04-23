#include "CInventoryButton.h"
#include "CShoesItem.h"
#include "CInput.h"
#include "CSpriteRenderer.h"

#define XSIZE 0.54f
#define YSIZE 0.525f

InventoryButton::InventoryButton()
	: Button(eUIType::Button)
	, mbDrop(false)
{
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

	Vector3 mPos = this->GetComponent<Transform>()->GetPosition();
	Vector3 mScale = this->GetComponent<Transform>()->GetScale();
	// test
	{
		ShoesItem* item = new ShoesItem(L"SmileTexture");
		item->Initalize();

		Vector2 size = item->GetItemSlotSize();
		Transform* tr = item->GetComponent<Transform>();
		tr->SetScale(Vector3(size.x * XSIZE, size.y * YSIZE, 1.0f));
		tr->SetPosition(Vector3(mPos.x - (mScale.x * 0.5f) + (XSIZE * 1), mPos.y + (mScale.y * 0.5f) - (YSIZE * 1), 1.0f));

		AddItem(item);

		int x = 0;
		int y = 0;

		item->SetIndex(x, y);
		mPoketSlot[0][0] = 1;
		mPoketSlot[0][1] = 1;

		mPoketSlot[1][0] = 1;
		mPoketSlot[1][1] = 1;

		item->SetOnInventory(true);
	}

	// test
	{
		ShoesItem* item = new ShoesItem(L"SmileTexture");
		item->Initalize();

		Vector2 size = item->GetItemSlotSize();
		Transform* tr = item->GetComponent<Transform>();
		tr->SetScale(Vector3(size.x * XSIZE, size.y * YSIZE, 1.0f));
		tr->SetPosition(Vector3(mPos.x - (mScale.x * 0.5f) + (XSIZE * 1), mPos.y + (mScale.y * 0.5f) - (YSIZE * 3), 1.0f));
		int x = 0;
		int y = 2;
		item->SetIndex(x, y);
		AddItem(item);

		mPoketSlot[2][0] = 1;
		mPoketSlot[2][1] = 1;

		mPoketSlot[3][0] = 1;
		mPoketSlot[3][1] = 1;

		item->SetOnInventory(true);
	}
}

void InventoryButton::Update()
{
	Button::Update();

	for (GameObject* item : mPoketItem)
	{
		if (item == nullptr)
			continue;

		item->Update();
	}

	if (/*GetPointToRect() > 0 && */Input::GetInstance()->GetMouseItemPick() == true)
	{
		ItemBase* item = Input::GetInstance()->GetPickItem();


		if (item == nullptr)
			return;

		ItemPushTop(item);

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
			item->SetOnInventory(true);
		}
		else
		{
			item->SetOnInventory(false);
		}

		// 아이템 드랍 가능한지 여부
		if (InvenPos.x - (InvenScale.x * 0.5f) <= itemPos.x - (itemScale.x * 0.5f)
			&& InvenPos.x + (InvenScale.x * 0.5f) >= itemPos.x - (itemScale.x * 0.5f)
			&& InvenPos.y - (InvenScale.y * 0.5f) <= itemPos.y + (itemScale.y * 0.5f)
			&& InvenPos.y + (InvenScale.y * 0.5f) >= itemPos.y + (itemScale.y * 0.5f))
		{
			float itemX = (itemPos.x - (itemScale.x * 0.5f));
			float itemY = itemPos.y + (itemScale.y * 0.5f);

			float InvenX = (InvenPos.x - (InvenScale.x * 0.5f));
			float InvenY = (InvenPos.y + (InvenScale.y * 0.5f));

			Vector2 idx{ itemX - InvenX , InvenY - itemY };

			// X 인덱스
			int X = idx.x / XSIZE;

			// Y 인덱스
			int Y = -1;
			if (itemY <= InvenY - (YSIZE * 0) && itemY > InvenY - (YSIZE * 1))
			{
				Y = 0;
			}
			else if (itemY <= InvenY - (YSIZE * 1) && itemY > InvenY - (YSIZE * 2))
			{
				Y = 1;
			}
			else if (itemY <= InvenY - (YSIZE * 2) && itemY > InvenY - (YSIZE * 3))
			{
				Y = 2;
			}
			else
			{
				Y = 3;
			}

			mbDrop = CheckPoekySlot(X, Y);

			if (mbDrop)
			{
				mXIndex = X;
				mYIndex = Y;

				item->SetIndex(mXIndex, mYIndex);
			}
		}
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

void InventoryButton::OnActive()
{
	mbEnable = true;

	for (UiBase* child : mChilds)
	{
		if (child == nullptr)
			continue;

		child->OnActive();
	}

	for (ItemBase* item : mPoketItem)
	{
		if (item == nullptr)
			continue;

		SpriteRenderer* sr = item->GetComponent<SpriteRenderer>();
		sr->SetRenderStop(false);
	}
}

void InventoryButton::UnActive()
{
	mbEnable = false;

	for (UiBase* child : mChilds)
	{
		if (child == nullptr)
			continue;

		child->UnActive();
	}

	for (ItemBase* item : mPoketItem)
	{
		if (item == nullptr)
			continue;

		SpriteRenderer* sr = item->GetComponent<SpriteRenderer>();
		sr->SetRenderStop(true);
	}
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
				return false;

			if (x + i >= mPoketSlot[y + j].size())
				return false;

			if (mPoketSlot[y + j][x + i] == 1)
			{
				return false;
			}
		}
	}
	return true;
}

void InventoryButton::DropItem(ItemBase* item)
{
	if (!mbDrop || mXIndex < 0 || mYIndex < 0)
		return;

	if (mbDrop)
	{
		Vector2 SlotNum = item->GetItemSlotSize();
		int SlotX = (int)SlotNum.x;
		int SlotY = (int)SlotNum.y;

		vector<pair<int, int>> index;
		for (int i = 0; i < SlotX; ++i)
		{
			for (int j = 0; j < SlotY; ++j)
			{
				if (mYIndex + j >= mPoketSlot.size())
					continue;

				if (mXIndex + i >= mPoketSlot[mYIndex + j].size())
					continue;

				//mPoketSlot[mYIndex + j][mXIndex + i] = 1;
				index.emplace_back(make_pair(i, j));
			}
		}

		if (index.size() == SlotX + SlotY)
		{
			for (int i = 0; i < index.size(); ++i)
			{
				mPoketSlot[mYIndex + index[i].second][mXIndex + index[i].first] = 1;
			}
		}

		SetDrop(false);
	}

	Transform* ItemTr = item->GetComponent<Transform>();
	Vector3 ItemPos = ItemTr->GetPosition();
	Vector3 ItemScale = ItemTr->GetScale();

	Transform* InvenTr = this->GetComponent<Transform>();
	Vector3 InvenPos = InvenTr->GetPosition();
	Vector3 InvenScale = InvenTr->GetScale();

	ItemPos.x = (InvenPos.x - (InvenScale.x * 0.5f)) + (XSIZE * mXIndex) + (ItemScale.x * 0.5f);
	ItemPos.y = (InvenPos.y + (InvenScale.y * 0.5f)) - (YSIZE * mYIndex) - (ItemScale.y * 0.5f);

	ItemTr->SetPosition(ItemPos);

	mXIndex = -1.f;
	mYIndex = -1.f;
}

void InventoryButton::AddItem(ItemBase* item)
{
	for (ItemBase* Item : mPoketItem)
	{
		if (Item == item)
			return;
	}

	mPoketItem.emplace_back(item);
	item->SetInventory(this);
}

bool InventoryButton::DeleteItem(ItemBase* item)
{
	vector<ItemBase*>::iterator iter;
	for (iter = mPoketItem.begin(); iter != mPoketItem.end(); ++iter)
	{
		if (*iter == item)
		{
			iter = mPoketItem.erase(iter);
			item->SetInventory(nullptr);
			return true;
		}
	}

	return false;
}

void InventoryButton::ItemPushTop(ItemBase* item)
{
	vector<ItemBase*>::iterator iter;
	for (iter = mPoketItem.begin(); iter != mPoketItem.end(); ++iter)
	{
		if (*iter == item)
		{
			mPoketItem.erase(iter);
			mPoketItem.emplace_back(item);
			return;
		}
	}
}

void InventoryButton::ClearPocketSlot(ItemBase* item)
{
	// 인벤토리 밖 월드에 드랍할때 인벤토리
	// 컨테이너에서 지워준다
	/*if (!DeleteItem(item))
		return;*/

	//Transform* itemTr = item->GetComponent<Transform>();
	//Vector3 itemPos = itemTr->GetPosition();
	//Vector3 itemScale = itemTr->GetScale();

	//Transform* InvenTr = this->GetComponent<Transform>();
	//Vector3 InvenPos = InvenTr->GetPosition();
	//Vector3 InvenScale = InvenTr->GetScale();


	//Vector2 SlotNum = item->GetItemSlotSize();
	//int SlotX = (int)SlotNum.x;
	//int SlotY = (int)SlotNum.y;

	//float itemX = (itemPos.x - (itemScale.x * 0.5f));
	//float itemY = itemPos.y + (itemScale.y * 0.5f);

	//float InvenX = (InvenPos.x - (InvenScale.x * 0.5f));
	//float InvenY = (InvenPos.y + (InvenScale.y * 0.5f));

	//Vector2 idx{ itemX - InvenX , InvenY - itemY };

	//// X 인덱스
	//int X = idx.x / XSIZE;

	//// Y 인덱스
	//int Y = -1;
	//if (itemY <= InvenY - (YSIZE * 0) && itemY > InvenY - (YSIZE * 1))
	//{
	//	Y = 0;
	//}
	//else if (itemY <= InvenY - (YSIZE * 1) && itemY > InvenY - (YSIZE * 2))
	//{
	//	Y = 1;
	//}
	//else if (itemY <= InvenY - (YSIZE * 2) && itemY > InvenY - (YSIZE * 3))
	//{
	//	Y = 2;
	//}
	//else
	//{
	//	Y = 3;
	//}

	Vector2 SlotNum = item->GetItemSlotSize();
	int SlotX = (int)SlotNum.x;
	int SlotY = (int)SlotNum.y;

	Vector2 Index = item->GetIndex();
	int indexX = Index.x;
	int indexY = Index.y;

	for (int i = 0; i < SlotY; ++i)
	{
		for (int j = 0; j < SlotX; ++j)
		{
			mPoketSlot[indexY + i][indexX + j] = 0;
		}
	}
}