#include "CEquipmentButton.h"
#include "CItemBase.h"
#include "CInput.h"
#include "CSpriteRenderer.h"
#include "CResourceManager.h"
#include "CShoesItem.h"

EquipmentButton::EquipmentButton(eEquipmentType type)
	: InventoryButton()
	, mType(type)
	, mbDrop(false)
	, mbOnRender(false)
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
		Load<Texture2D>(L"ItemSlot", L"UI//NoneCanvers.png");

	material->SetTexture(eTextureSlot::T0, tex);

	sr->SetMesh(mesh);
	sr->SetMaterial(material);
}

void EquipmentButton::Update()
{
	Button::Update();

	if (Input::GetInstance()->GetMouseItemPick())
	{
		ItemBase* item = Input::GetInstance()->GetPickItem();

		if (item == nullptr)
			return;

		// 아이템을 집고 있는상태이면 충돌 체크를 한다
		Transform* itemTr = item->GetComponent<Transform>();
		Vector3 itemPos = itemTr->GetPosition();
		Vector3 itemScale = itemTr->GetScale();

		Transform* InvenTr = this->GetComponent<Transform>();
		Vector3 InvenPos = InvenTr->GetPosition();
		Vector3 InvenScale = InvenTr->GetScale();

		if (InvenPos.x - (InvenScale.x * 0.5f) <= itemPos.x
			&& InvenPos.x + (InvenScale.x * 0.5f) >= itemPos.x
			&& InvenPos.y - (InvenScale.y * 0.5f) <= itemPos.y
			&& InvenPos.y + (InvenScale.y * 0.5f) >= itemPos.y)
		{
			SetPointToRect(1);

			// 충돌 한경우 아이템 타입과 슬롯의 타입을 비교후
			// 일치하는경우만 드롭이 가능
			mbOnRender = true;
			if (item->GetItemType() == mType)
			{
				mItem = item;
				mbDrop = true;
				item->SetSlotInventory(this);
				item->SetDrop(mbDrop);
			}
			else
			{
				mItem = nullptr;
				mbDrop = false;
				item->SetSlotInventory(nullptr);
				item->SetDrop(mbDrop);
			}
		}
		else
		{
			mbOnRender = false;

			// 같은 타입의 슬롯들이 많을시 버그로 인하여
			// 슬롯이 갖고 있는 아이템과 아이템이 갖고 있는 슬롯이
			// 일치할때 초기화
			if (item == mItem && item->GetSlotInventory() == this)
			{
				mItem == nullptr;
				item->SetSlotInventory(nullptr);
				item->SetDrop(mbDrop);
			}
		}
	}
	else
	{
		// 아이템이 없으면 투명하게 렌더링
		mbOnRender = false;
	}
}

void EquipmentButton::FixedUpdate()
{
	Button::FixedUpdate();
}

void EquipmentButton::Render()
{
	SpriteRenderer* sr = GetComponent<SpriteRenderer>();
	Vector4 color = Vector4(1.0f, 0.0f, 0.0f, 0.3f);

	if (mbDrop) // 드롭가능한경우 녹색렌더
		color = Vector4(0.0f, 1.0f, 0.0f, 0.3f);
	else		// 드롭불가능한경우 빨강렌더
		color = Vector4(1.0f, 0.0f, 0.0f, 0.3f);

	if (!mbOnRender)// 렌더할 필요없을때 투명 렌더
		color = Vector4(0.0f, 0.0f, 0.0f, 0.0f);

	sr->GetMaterial()->SetData(eGpuParam::Vector4, &color);

	Button::Render();
}

void EquipmentButton::Click()
{
}

void EquipmentButton::DropItem(ItemBase* item)
{
	// 아이템 위치를 슬롯중앙 포지션으로 변경
	Transform* tr = GetComponent<Transform>();
	Vector3 pos = tr->GetPosition();

	Transform* itemTr = item->GetComponent<Transform>();
	itemTr->SetPosition(pos);

	item->SetOnInventory(true);
	item->SetPick(false);

	InventoryButton* inven = item->GetInventory();
	if (inven != nullptr)
	{
		inven->DeleteItem(item);
	}
}

void EquipmentButton::ClearPocketSlot(ItemBase* item)
{
	item->SetSlotInventory(nullptr);
}
