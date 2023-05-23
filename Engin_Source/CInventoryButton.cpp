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
#include "CMpPotionItem.h"
#include "CHpPotionItem.h"

InventoryButton::InventoryButton()
	: Button(eUIType::Button)
	, mbDrop(false)
	, mXIndex(0)
	, mYIndex(0)
{
}

InventoryButton::~InventoryButton()
{
	
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
	Vector3 mScale = this->GetComponent<Transform>()->GetScale() * this->GetComponent<Transform>()->GetSize();

	// ���Ƿ� �迭���ο� �������� ä���ش�
	//test
	{
		ShoesItem* item = new ShoesItem(L"SmileTexture");
		item->Initalize();
		item->SetInventory(this);

		Vector2 size = item->GetItemSlotSize();
		Transform* tr = item->GetComponent<Transform>();
		tr->SetSize(Vector3(size.x * ITEM_X_SIZE, size.y * ITEM_Y_SIZE, 1.0f));
		tr->SetPosition(Vector3(mPos.x - (mScale.x * 0.5f) + (ITEM_X_SIZE * 1), mPos.y + (mScale.y * 0.5f) - (ITEM_Y_SIZE * 1), 1.0f));

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
		item->SetInventory(this);

		Vector2 size = item->GetItemSlotSize();
		Transform* tr = item->GetComponent<Transform>();
		tr->SetSize(Vector3(size.x * ITEM_X_SIZE, size.y * ITEM_Y_SIZE, 1.0f));
		tr->SetPosition(Vector3(mPos.x - (mScale.x * 0.5f) + (ITEM_X_SIZE * 1), mPos.y + (mScale.y * 0.5f) - (ITEM_Y_SIZE * 3), 1.0f));
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
		item->SetInventory(this);

		Vector2 size = item->GetItemSlotSize();
		Transform* tr = item->GetComponent<Transform>();
		tr->SetSize(Vector3(size.x * ITEM_X_SIZE, size.y * ITEM_Y_SIZE, 1.0f));

		int x = 2;
		int y = 0;

		// ���� �ε������� ��µ� ��ġ�� ����ϴ� ��
		// ������Ʈ�� �ε��� + (������Ʈ�� ũ�� / 2 )  Ex) x = 2 + (2 * 0.5f), y = 0 + (2 * 0.5f)
		float xSize = x + (size.x * 0.5f);
		float ySize = y + (size.y * 0.5f);

		// ������Ʈ ũ���� ���ݸ�ŭ �������� ��ġ���ش�
		// ---------
		// |  ��   |
		// |�� 0   |
		// |       |
		// ---------
		tr->SetPosition(Vector3(mPos.x - (mScale.x * 0.5f) + (ITEM_X_SIZE * xSize)
			, mPos.y + (mScale.y * 0.5f) - (ITEM_Y_SIZE * ySize)
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
		item->SetInventory(this);

		Vector2 size = item->GetItemSlotSize();
		Transform* tr = item->GetComponent<Transform>();
		tr->SetSize(Vector3(size.x * ITEM_X_SIZE, size.y * ITEM_Y_SIZE, 1.0f));

		int x = 2;
		int y = 3;
		float xSize = x + (size.x * 0.5f);
		float ySize = y + (size.y * 0.5f);
		tr->SetPosition(Vector3(mPos.x - (mScale.x * 0.5f) + (ITEM_X_SIZE * xSize)
			, mPos.y + (mScale.y * 0.5f) - (ITEM_Y_SIZE * ySize)
			, 1.0f));

		item->SetIndex(x, y);
		AddItem(item);

		mPoketSlot[3][2] = 1;
		mPoketSlot[3][3] = 1;

		item->SetOnInventory(true);
		ItemManager::GetInstance()->AddItem(item);
	}

	{
		CapItem* item = new CapItem(L"SmileTexture");
		item->Initalize();
		item->SetInventory(this);

		Vector2 size = item->GetItemSlotSize();
		Transform* tr = item->GetComponent<Transform>();
		tr->SetSize(Vector3(size.x * ITEM_X_SIZE, size.y * ITEM_Y_SIZE, 1.0f));

		int x = 4;
		int y = 2;
		float xSize = x + (size.x * 0.5f);
		float ySize = y + (size.y * 0.5f);
		tr->SetPosition(Vector3(mPos.x - (mScale.x * 0.5f) + (ITEM_X_SIZE * xSize)
			, mPos.y + (mScale.y * 0.5f) - (ITEM_Y_SIZE * ySize)
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
		item->SetInventory(this);

		Vector2 size = item->GetItemSlotSize();
		Transform* tr = item->GetComponent<Transform>();
		tr->SetSize(Vector3(size.x * ITEM_X_SIZE, size.y * ITEM_Y_SIZE, 1.0f));

		int x = 6;
		int y = 0;
		float xSize = x + (size.x * 0.5f);
		float ySize = y + (size.y * 0.5f);
		tr->SetPosition(Vector3(mPos.x - (mScale.x * 0.5f) + (ITEM_X_SIZE * xSize)
			, mPos.y + (mScale.y * 0.5f) - (ITEM_Y_SIZE * ySize)
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
		item->SetInventory(this);

		Vector2 size = item->GetItemSlotSize();
		Transform* tr = item->GetComponent<Transform>();
		tr->SetSize(Vector3(size.x * ITEM_X_SIZE, size.y * ITEM_Y_SIZE, 1.0f));

		int x = 7;
		int y = 0;
		float xSize = x + (size.x * 0.5f);
		float ySize = y + (size.y * 0.5f);
		tr->SetPosition(Vector3(mPos.x - (mScale.x * 0.5f) + (ITEM_X_SIZE * xSize)
			, mPos.y + (mScale.y * 0.5f) - (ITEM_Y_SIZE * ySize)
			, 1.0f));

		item->SetIndex(x, y);
		AddItem(item);

		mPoketSlot[0][7] = 1;
		mPoketSlot[1][7] = 1;
		mPoketSlot[2][7] = 1;

		item->SetOnInventory(true);
		ItemManager::GetInstance()->AddItem(item);
	}

	{
		HpPotionItem* item = new HpPotionItem();
		item->Initalize();
		item->SetInventory(this);

		item->SetName(L"HpPotion1");

		Vector2 size = item->GetItemSlotSize();
		Transform* tr = item->GetComponent<Transform>();
		tr->SetSize(Vector3(size.x * ITEM_X_SIZE, size.y * ITEM_Y_SIZE, 1.0f));

		int x = 4;
		int y = 0;
		float xSize = x + (size.x * 0.5f);
		float ySize = y + (size.y * 0.5f);
		tr->SetPosition(Vector3(mPos.x - (mScale.x * 0.5f) + (ITEM_X_SIZE * xSize)
			, mPos.y + (mScale.y * 0.5f) - (ITEM_Y_SIZE * ySize)
			, 1.0f));

		item->SetIndex(x, y);
		AddItem(item);

		mPoketSlot[0][4] = 1;

		item->SetOnInventory(true);
		ItemManager::GetInstance()->AddItem(item);
	}

	{
		HpPotionItem* item = new HpPotionItem();
		item->Initalize();
		item->SetName(L"HpPotion2");
		item->SetInventory(this);

		Vector2 size = item->GetItemSlotSize();
		Transform* tr = item->GetComponent<Transform>();
		tr->SetSize(Vector3(size.x * ITEM_X_SIZE, size.y * ITEM_Y_SIZE, 1.0f));

		int x = 5;
		int y = 0;
		float xSize = x + (size.x * 0.5f);
		float ySize = y + (size.y * 0.5f);
		tr->SetPosition(Vector3(mPos.x - (mScale.x * 0.5f) + (ITEM_X_SIZE * xSize)
			, mPos.y + (mScale.y * 0.5f) - (ITEM_Y_SIZE * ySize)
			, 1.0f));

		item->SetIndex(x, y);
		AddItem(item);

		mPoketSlot[0][5] = 1;

		item->SetOnInventory(true);
		ItemManager::GetInstance()->AddItem(item);
	}

	{
		MpPotionItem* item = new MpPotionItem();
		item->Initalize();
		item->SetName(L"MpPotion1");
		item->SetInventory(this);

		Vector2 size = item->GetItemSlotSize();
		Transform* tr = item->GetComponent<Transform>();
		tr->SetSize(Vector3(size.x * ITEM_X_SIZE, size.y * ITEM_Y_SIZE, 1.0f));

		int x = 4;
		int y = 1;
		float xSize = x + (size.x * 0.5f);
		float ySize = y + (size.y * 0.5f);
		tr->SetPosition(Vector3(mPos.x - (mScale.x * 0.5f) + (ITEM_X_SIZE * xSize)
			, mPos.y + (mScale.y * 0.5f) - (ITEM_Y_SIZE * ySize)
			, 1.0f));

		item->SetIndex(x, y);
		AddItem(item);

		mPoketSlot[1][4] = 1;

		item->SetOnInventory(true);
		ItemManager::GetInstance()->AddItem(item);
	}

	{
		MpPotionItem* item = new MpPotionItem();
		item->Initalize();
		item->SetName(L"MpPotion2");
		item->SetInventory(this);

		Vector2 size = item->GetItemSlotSize();
		Transform* tr = item->GetComponent<Transform>();
		tr->SetSize((Vector3(size.x * ITEM_X_SIZE, size.y * ITEM_Y_SIZE, 1.0f)));

		int x = 5;
		int y = 1;
		float xSize = x + (size.x * 0.5f);
		float ySize = y + (size.y * 0.5f);
		tr->SetPosition(Vector3(mPos.x - (mScale.x * 0.5f) + (ITEM_X_SIZE * xSize)
			, mPos.y + (mScale.y * 0.5f) - (ITEM_Y_SIZE * ySize)
			, 1.0f));

		item->SetIndex(x, y);
		AddItem(item);

		mPoketSlot[1][5] = 1;

		item->SetOnInventory(true);
		ItemManager::GetInstance()->AddItem(item);
	}
}

void InventoryButton::Update()
{
	DeleteOnWolrdItem();

	if (Input::GetInstance()->GetMouseItemPick() == true)
	{
		ItemBase* item = Input::GetInstance()->GetPickItem();

		if (item == nullptr)
			return;

		// ���콺�� ���� �������� �ִ°�� �浹 üũ�� ����

		Transform* itemTr = item->GetComponent<Transform>();
		Vector3 itemPos = itemTr->GetPosition();
		Vector3 itemScale = itemTr->GetScale() * itemTr->GetSize();

		Transform* InvenTr = this->GetComponent<Transform>();
		Vector3 InvenPos = InvenTr->GetPosition();
		Vector3 InvenScale = InvenTr->GetScale() * InvenTr->GetSize();

		if (InvenPos.x - (InvenScale.x * 0.5f) <= itemPos.x + (itemScale.x * 0.5f)
			&& InvenPos.x + (InvenScale.x * 0.5f) >= itemPos.x - (itemScale.x * 0.5f)
			&& InvenPos.y - (InvenScale.y * 0.5f) <= itemPos.y + (itemScale.y * 0.5f)
			&& InvenPos.y + (InvenScale.y * 0.5f) >= itemPos.y - (itemScale.y * 0.5f))
		{
			// �浹 O
			SetPointToRect(1);
			item->SetOnInventory(true);
			item->SetInventory(this);
		}
		else
		{
			// �浹 X
			item->SetOnInventory(false);
			item->SetInventory(nullptr);
		}

		// ������ ��� �������� ����
		if (InvenPos.x - (InvenScale.x * 0.5f) <= itemPos.x - (itemScale.x * 0.5f)
			&& InvenPos.x + (InvenScale.x * 0.5f) >= itemPos.x - (itemScale.x * 0.5f)
			&& InvenPos.y - (InvenScale.y * 0.5f) <= itemPos.y + (itemScale.y * 0.5f)
			&& InvenPos.y + (InvenScale.y * 0.5f) >= itemPos.y + (itemScale.y * 0.5f))
		{
			float itemX = (itemPos.x - (itemScale.x * 0.5f)) + ITEM_X_SIZE * 0.25f; // LeftTop���� Ÿ�� ��ĭ�� ���ݸ�ŭ
			float itemY = (itemPos.y + (itemScale.y * 0.5f)) - ITEM_Y_SIZE * 0.5f;  // ��ġ���� �ε����� üũ �Ѵ�
																					 // ���Ǽ� ��� ȿ��		
			float InvenX = (InvenPos.x - (InvenScale.x * 0.5f));
			float InvenY = (InvenPos.y + (InvenScale.y * 0.5f));

			Vector2 idx{ itemX - InvenX , InvenY - itemY };

			// X �ε���
			int X = idx.x / ITEM_X_SIZE;

			// Y �ε���
			int Y = -1;
			if (itemY <= InvenY - (ITEM_Y_SIZE * 0) && itemY > InvenY - (ITEM_Y_SIZE * 1))
			{
				Y = 0;
			}
			else if (itemY <= InvenY - (ITEM_Y_SIZE * 1) && itemY > InvenY - (ITEM_Y_SIZE * 2))
			{
				Y = 1;
			}
			else if (itemY <= InvenY - (ITEM_Y_SIZE * 2) && itemY > InvenY - (ITEM_Y_SIZE * 3))
			{
				Y = 2;
			}
			else
			{
				Y = 3;
			}

			// ���� �ε����κ��� ������ ũ�⸸ŭ�� �迭�� ����ִ��� Ȯ����
			// ��������ʴٸ� Drop = false
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

	item->SetIndex(x, y);
	return true;
}

bool InventoryButton::CheckPoekySlot(ItemBase* item, int& x, int& y)
{
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

	mXIndex = x;
	mYIndex = y;
	item->SetIndex(x, y);
	return true;
}

bool InventoryButton::PickUpItem(ItemBase* item)
{
	Vector2 slot = item->GetItemSlotSize();
	int slotX = slot.x;
	int slotY = slot.y;

	for (int i = 0; i < mPoketSlot.size(); ++i)
	{
		for (int j = 0; j < mPoketSlot[i].size(); ++j)
		{
			if (CheckPoekySlot(item, j, i))
			{
				mbDrop = true;
				Transform* itemTr = item->GetComponent<Transform>();
				itemTr->SetSize(item->GetInvenSize());

				DropItem(item);
				return true;
			}
		}
	}
	return false;
}

void InventoryButton::DropItem(ItemBase* item)
{
	if (!mbDrop || mXIndex < 0 || mYIndex < 0)
		return;


	Vector2 SlotNum = item->GetItemSlotSize();
	int SlotX = (int)SlotNum.x;
	int SlotY = (int)SlotNum.y;

	// ��ȸ ���� �ε����ʰ� �ΰ�� �������� Ǫ������ ����
	// ��ȸ�� �����ϰ� �̷������� �������� Ǫ��
	// �׷��ؼ� �ε����� �����̳ʿ� ��Ɗx�� �������� �迭�� ä���ش�
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

	// �ʱ�ȭ
	SetDrop(false);

	// �κ��丮 �� �̹Ƿ� ������ ����
	item->SetOnInventory(true);

	// ���� ������ ����
	item->SetPick(false);

	// �̹� �������� �������� ����Ҽ� �ִ�
	// ���ǿ��� ����
	item->SetDrop(false);

	// �κ��丮 ���ο� �������� ��������Ƿ�
	// ������ �����ִ� �����������͸� �����ش�
	item->SetSlotInventory(nullptr);

	//
	item->SetStage(false);

	// �������� ��� �����̳ʿ� �־��ش�
	AddItem(item);

	SpriteRenderer* itemSr = item->GetComponent<SpriteRenderer>();
	if (itemSr != nullptr)
	{
		if (mbEnable)
		{
			itemSr->SetRenderStop(false);
		}
		else
		{
			itemSr->SetRenderStop(true);
		}
	}


	Transform* ItemTr = item->GetComponent<Transform>();
	Vector3 ItemPos = ItemTr->GetPosition();
	Vector3 ItemScale = Vector3(item->GetInvenSize().x, item->GetInvenSize().y, 0.0f);

	Transform* InvenTr = this->GetComponent<Transform>();
	Vector3 InvenPos = InvenTr->GetPosition();
	Vector3 InvenScale = InvenTr->GetScale() * InvenTr->GetSize();

	ItemPos.x = (InvenPos.x - (InvenScale.x * 0.5f)) + (ITEM_X_SIZE * mXIndex) + (ItemScale.x * 0.5f);
	ItemPos.y = (InvenPos.y + (InvenScale.y * 0.5f)) - (ITEM_Y_SIZE * mYIndex) - (ItemScale.y * 0.5f);

	ItemTr->SetPosition(ItemPos);
	ItemTr->SetSize(item->GetInvenSize());

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

void InventoryButton::DeleteOnWolrdItem()
{
	queue<ItemBase*> deleteQueue;
	for (ItemBase* item : mPoketItem)
	{
		if (item == nullptr)
			continue;

		if (item->GetStage())
		{
			deleteQueue.push(item);
		}
	}

	while (!deleteQueue.empty())
	{
		ItemBase* item = deleteQueue.front();
		deleteQueue.pop();

		DeleteItem(item);
	}
}

void InventoryButton::ClearPocketSlot(ItemBase* item)
{
	// �κ��丮 �� ���忡 ����Ҷ� �κ��丮
	// �����̳ʿ��� �����ش�
	//if (!DeleteItem(item))
	//	return;

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

	//// X �ε���
	//int X = idx.x / XSIZE;

	//// Y �ε���
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


	// �������� ���� �ִ� �ε������� ũ�� ���� �迭�� ����ش�
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