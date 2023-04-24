#include "CInventoryButton.h"
#include "CInput.h"
#include "CSpriteRenderer.h"
#include "CShoesItem.h"
#include "CGloveItem.h"
#include "CBeltItem.h"
#include "CSuitItem.h"
#include "CCapItem.h"
#include "CWeponItem.h"
#include "CItemManager.h"

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

	// 임의로 배열내부에 아이템을 채워준다
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
		ItemManager::GetInstance()->AddItem(item);
	}

	// test
	{
		GloveItem* item = new GloveItem(L"SmileTexture");
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
		ItemManager::GetInstance()->AddItem(item);
	}

	{
		SuitItem* item = new SuitItem(L"SmileTexture");
		item->Initalize();

		Vector2 size = item->GetItemSlotSize();
		Transform* tr = item->GetComponent<Transform>();
		tr->SetScale(Vector3(size.x * XSIZE, size.y * YSIZE, 1.0f));

		int x = 2;
		int y = 0;

		// 현재 인덱스부터 출력될 위치를 계산하는 식
		// 오브젝트의 인덱스 + (오브젝트의 크기 / 2 )  Ex) x = 2 + (2 * 0.5f), y = 0 + (2 * 0.5f)
		float xSize = x + (size.x * 0.5f);
		float ySize = y + (size.y * 0.5f);

		// 오브젝트 크기의 절반만큼 포지션을 위치해준다
		// ---------
		// |  ↓   |
		// |→ 0   |
		// |       |
		// ---------
		tr->SetPosition(Vector3(mPos.x - (mScale.x * 0.5f) + (XSIZE * xSize)
			, mPos.y + (mScale.y * 0.5f) - (YSIZE *  ySize)
			, 1.0f));

		item->SetIndex(x, y);
		AddItem(item);

		mPoketSlot[0][2] = 1;
		mPoketSlot[1][2] = 1;
		mPoketSlot[2][2] = 1;

		mPoketSlot[0][3] = 1;
		mPoketSlot[1][3] = 1;
		mPoketSlot[2][3] = 1;

		item->SetOnInventory(true);
		ItemManager::GetInstance()->AddItem(item);
	}

	{
		BeltItem* item = new BeltItem(L"SmileTexture");
		item->Initalize();

		Vector2 size = item->GetItemSlotSize();
		Transform* tr = item->GetComponent<Transform>();
		tr->SetScale(Vector3(size.x * XSIZE, size.y * YSIZE, 1.0f));
	
		int x = 4;
		int y = 0;
		float xSize = x + (size.x * 0.5f);
		float ySize = y + (size.y * 0.5f);
		tr->SetPosition(Vector3(mPos.x - (mScale.x * 0.5f) + (XSIZE * xSize)
			, mPos.y + (mScale.y * 0.5f) - (YSIZE *  ySize)
			, 1.0f));

		item->SetIndex(x, y);
		AddItem(item);

		mPoketSlot[0][4] = 1;
		mPoketSlot[0][5] = 1;

		item->SetOnInventory(true);
		ItemManager::GetInstance()->AddItem(item);
	}

	{
		CapItem* item = new CapItem(L"SmileTexture");
		item->Initalize();

		Vector2 size = item->GetItemSlotSize();
		Transform* tr = item->GetComponent<Transform>();
		tr->SetScale(Vector3(size.x * XSIZE, size.y * YSIZE, 1.0f));

		int x = 4;
		int y = 2;
		float xSize = x + (size.x * 0.5f);
		float ySize = y + (size.y * 0.5f);
		tr->SetPosition(Vector3(mPos.x - (mScale.x * 0.5f) + (XSIZE * xSize)
			, mPos.y + (mScale.y * 0.5f) - (YSIZE * ySize)
			, 1.0f));

		item->SetIndex(x, y);
		AddItem(item);

		mPoketSlot[2][4] = 1;
		mPoketSlot[2][5] = 1;

		mPoketSlot[3][4] = 1;
		mPoketSlot[3][5] = 1;

		item->SetOnInventory(true);
		ItemManager::GetInstance()->AddItem(item);
	}

	{
		WeponItem* item = new WeponItem(L"SmileTexture");
		item->Initalize();

		Vector2 size = item->GetItemSlotSize();
		Transform* tr = item->GetComponent<Transform>();
		tr->SetScale(Vector3(size.x * XSIZE, size.y * YSIZE, 1.0f));

		int x = 6;
		int y = 0;
		float xSize = x + (size.x * 0.5f);
		float ySize = y + (size.y * 0.5f);
		tr->SetPosition(Vector3(mPos.x - (mScale.x * 0.5f) + (XSIZE * xSize)
			, mPos.y + (mScale.y * 0.5f) - (YSIZE * ySize)
			, 1.0f));

		item->SetIndex(x, y);
		AddItem(item);

		mPoketSlot[0][6] = 1;
		mPoketSlot[1][6] = 1;
		mPoketSlot[2][6] = 1;

		item->SetOnInventory(true);
		ItemManager::GetInstance()->AddItem(item);
	}

	{
		WeponItem* item = new WeponItem(L"SmileTexture");
		item->Initalize();

		Vector2 size = item->GetItemSlotSize();
		Transform* tr = item->GetComponent<Transform>();
		tr->SetScale(Vector3(size.x * XSIZE, size.y * YSIZE, 1.0f));

		int x = 7;
		int y = 0;
		float xSize = x + (size.x * 0.5f);
		float ySize = y + (size.y * 0.5f);
		tr->SetPosition(Vector3(mPos.x - (mScale.x * 0.5f) + (XSIZE * xSize)
			, mPos.y + (mScale.y * 0.5f) - (YSIZE * ySize)
			, 1.0f));

		item->SetIndex(x, y);
		AddItem(item);

		mPoketSlot[0][7] = 1;
		mPoketSlot[1][7] = 1;
		mPoketSlot[2][7] = 1;

		item->SetOnInventory(true);
		ItemManager::GetInstance()->AddItem(item);
	}
}

void InventoryButton::Update()
{
	if (Input::GetInstance()->GetMouseItemPick() == true)
	{
		ItemBase* item = Input::GetInstance()->GetPickItem();

		if (item == nullptr)
			return;

		// 마우스로 집은 아이템이 있는경우 충돌 체크를 진행

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
			// 충돌 O
			SetPointToRect(1);
			item->SetOnInventory(true);
		}
		else
		{
			// 충돌 X
			item->SetOnInventory(false);
		}

		// 아이템 드랍 가능한지 여부
		if (InvenPos.x - (InvenScale.x * 0.5f) <= itemPos.x - (itemScale.x * 0.5f)
			&& InvenPos.x + (InvenScale.x * 0.5f) >= itemPos.x - (itemScale.x * 0.5f)
			&& InvenPos.y - (InvenScale.y * 0.5f) <= itemPos.y + (itemScale.y * 0.5f)
			&& InvenPos.y + (InvenScale.y * 0.5f) >= itemPos.y + (itemScale.y * 0.5f))
		{
			float itemX = (itemPos.x - (itemScale.x * 0.5f)) + XSIZE * 0.25f; // LeftTop에서 타일 한칸에 절반만큼
			float itemY = (itemPos.y + (itemScale.y * 0.5f)) - YSIZE * 0.5f;  // 위치부터 인덱스를 체크 한다
																			  // 편의성 상승 효과		
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

			// 현재 인덱스로부터 아이템 크기만큼의 배열이 비어있는지 확인후
			// 비어있지않다면 Drop = false
			mbDrop = CheckPoekySlot(X, Y);

			if (mbDrop)
			{
				mXIndex = X;
				mYIndex = Y;

				item->SetIndex(mXIndex, mYIndex);
			}
			item->SetDrop(mbDrop);
		}
	}

	Button::Update();
}

void InventoryButton::FixedUpdate()
{
	Button::FixedUpdate();
}

void InventoryButton::Render()
{
	Button::Render();
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

		// 순회 도중 인덱스초과 인경우 아이템을 푸쉬하지 않음
		// 순회가 완전하게 이루어진경우 아이템을 푸쉬
		// 그러해서 인덱스를 컨테이너에 담아둿다 마지막에 배열을 채워준다
		vector<pair<int, int>> index;
		for (int i = 0; i < SlotY; ++i)
		{
			for (int j = 0; j < SlotX; ++j)
			{
				if (mYIndex + i >= mPoketSlot.size())
					return;

				if (mXIndex + j >= mPoketSlot[mYIndex + i].size())
					return;

				//mPoketSlot[mYIndex + j][mXIndex + i] = 1;
				index.emplace_back(make_pair(j, i));
			}
		}

		if (index.size() == SlotX * SlotY)
		{
			for (int i = 0; i < index.size(); ++i)
			{
				mPoketSlot[mYIndex + index[i].second][mXIndex + index[i].first] = 1;
			}
		}

		// 초기화
		SetDrop(false);

		// 인벤토리 위 이므로 온전히 렌더
		item->SetOnInventory(true);

		// 픽을 내려둔 상태
		item->SetPick(false);

		// 이미 내려놓은 아이템은 드롭할수 있는
		// 존건에서 제외
		item->SetDrop(false);

		// 인벤토리 내부에 아이템을 드롭했으므로
		// 슬롯이 갖고있는 아이템포인터를 지워준다
		item->SetSlotInventory(nullptr);

		// 아이템을 담는 컨테이너에 넣어준다
		AddItem(item);
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
	// 아이템을 집은경우 제일 위에 렌더되어서
	// 이동중 다른 아이템보다 렌더 순위가 우선된다
	vector<ItemBase*>::iterator iter;
	for (iter = mPoketItem.begin(); iter != mPoketItem.end(); ++iter)
	{
		if (*iter == item)
		{
			mPoketItem.erase(iter);
			break;
		}
	}
	mPoketItem.emplace_back(item);
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


	// 아이템이 갖고 있는 인덱스부터 크기 까지 배열이 비워준다
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