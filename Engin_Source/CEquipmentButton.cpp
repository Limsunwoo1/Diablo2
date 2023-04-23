#include "CEquipmentButton.h"
#include "CItemBase.h"
#include "CInput.h"
#include "CSpriteRenderer.h"
#include "CResourceManager.h"

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

	SpriteRenderer* sr = this->AddComponent<SpriteRenderer>();
	std::shared_ptr<Mesh> mesh = ResourceManager::GetInstance()->Find<Mesh>(L"RectMesh");
	std::shared_ptr<Material> material = ResourceManager::GetInstance()->Find<Material>(L"ItemSlotMaterial");
	std::shared_ptr<Texture2D> tex = ResourceManager::GetInstance()->
		Load<Texture2D>(L"ItemSlot", L"UI//Title_01.png");

	material->SetTexture(eTextureSlot::T0, tex);

	sr->SetMesh(mesh);
	sr->SetMaterial(material);
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

		item->FixedUpdate();
	}
}

void EquipmentButton::Render()
{
	Button::Render();

	for (GameObject* item : mPoketItem)
	{
		if (item == nullptr)
			continue;

		item->Render();
	}
}

void EquipmentButton::Click()
{
}
