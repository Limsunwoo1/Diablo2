#include "CEquipmentButton.h"
#include "CItemBase.h"
#include "CInput.h"

EquipmentButton::EquipmentButton(eEquipmentType type)
	: InventoryButton()
	, mType(type)
{
}

EquipmentButton::~EquipmentButton()
{
}

void EquipmentButton::Initalize()
{
	Button::Initalize();
}

void EquipmentButton::Update()
{
	Button::Update();

	for (GameObject* item : mPoketItem)
	{
		if (item == nullptr)
			continue;

		item->Update();
	}

	if (Input::GetInstance()->GetMouseItemPick())
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
			
			if (item->GetItemType() == mType)
			{
				/*item->GetInventory()->DeleteItem(item);

				SetDrop(true);
				AddItem(item);*/
			}
			else
			{
				
			}
		}
	}
}

void EquipmentButton::FixedUpdate()
{
	Button::FixedUpdate();

	for (GameObject* item : mPoketItem)
	{
		if (item == nullptr)
			continue;

		item->Render();
	}
}

void EquipmentButton::Render()
{
	Button::Render();

	for (GameObject* item : mPoketItem)
	{
		if (item == nullptr)
			continue;

		item->Update();
	}
}

void EquipmentButton::Click()
{
}
