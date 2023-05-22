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
	, mbUsed(false) // true = ��񽽷Կ� ������ ������, false = ��� ���Կ� ���� X
	, mItem(nullptr)
{
}

EquipmentButton::~EquipmentButton()
{
}

void EquipmentButton::Initalize()
{
	Button::Initalize();

	SpriteRenderer* sr = this->AddComponent<SpriteRenderer>();
	std::weak_ptr<Mesh> mesh = ResourceManager::GetInstance()->Find<Mesh>(L"RectMesh");
	std::weak_ptr<Material> material = ResourceManager::GetInstance()->Find<Material>(L"ItemSlotMaterial");
	std::weak_ptr<Texture2D> tex = ResourceManager::GetInstance()->
		Load<Texture2D>(L"ItemSlot", L"UI//NoneCanvers.png");

	std::weak_ptr<Texture2D> tex1 = ResourceManager::GetInstance()->
		Find<Texture2D>(L"SmileTexture");

	material.lock()->SetTexture(eTextureSlot::T0, tex1);

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

		// �������� ���� �ִ»����̸� �浹 üũ�� �Ѵ�
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

			// �浹 �Ѱ�� ������ Ÿ�԰� ������ Ÿ���� ����
			// ��ġ�ϴ°�츸 ����� ����
			mbOnRender = true;
			if (item->GetItemType() == mType && mbUsed == false)
			{
				mItem = item;
				mbDrop = true;
				item->SetSlotInventory(this);
				item->SetDrop(mbDrop);
			}
			else
			{
				if (mbUsed)
				{
					mbDrop = false;
					item->SetDrop(mbDrop);
					return;
				}

				mItem = nullptr;

				if(item->GetSlotInventory() == this)
					item->SetSlotInventory(nullptr);

			}
		}
		else
		{
			mbOnRender = false;

			// ���� Ÿ���� ���Ե��� ������ ���׷� ���Ͽ�
			// ������ ���� �ִ� �����۰� �������� ���� �ִ� ������
			// ��ġ�Ҷ� �ʱ�ȭ
			if (item == mItem && item->GetSlotInventory() == this)
			{
				mItem = nullptr;
				item->SetSlotInventory(nullptr);
				item->SetDrop(false);
			}
		}
	}
	else
	{
		// �������� ������ �����ϰ� ������
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

	if (mbDrop) // ��Ӱ����Ѱ�� �������
		color = Vector4(0.0f, 1.0f, 0.0f, 0.3f);
	else		// ��ӺҰ����Ѱ�� ��������
		color = Vector4(1.0f, 0.0f, 0.0f, 0.3f);

	if (!mbOnRender)// ������ �ʿ������ ���� ����
		color = Vector4(0.0f, 0.0f, 0.0f, 0.0f);

	sr->GetMaterial().lock()->SetData(eGpuParam::Vector4, &color);

	Button::Render();
}

void EquipmentButton::Click()
{
}

void EquipmentButton::DropItem(ItemBase* item)
{
	// ������ ��ġ�� �����߾� ���������� ����
	Transform* tr = GetComponent<Transform>();
	Vector3 pos = tr->GetPosition();

	Transform* itemTr = item->GetComponent<Transform>();
	itemTr->SetPosition(pos);

	item->SetOnInventory(true);
	item->SetPick(false);

	// ���Կ� ��ӽ� �κ��丮�����͸� �����ش�
	InventoryButton* inven = item->GetInventory();
	if (inven != nullptr)
	{
		inven->DeleteItem(item);
	}

	// ���Կ� ���¾������� �����ͷ� �����ִ´�
	SetItem(item);
	SetUsed(true);
}

void EquipmentButton::ClearPocketSlot(ItemBase* item)
{
	item->SetSlotInventory(nullptr);
	SetUsed(false);
}
