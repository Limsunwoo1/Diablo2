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
