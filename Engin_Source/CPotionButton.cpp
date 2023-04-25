#include "CPotionButton.h"
#include "CInput.h"
#include "CItemBase.h"

PotionButton::PotionButton()
	: EquipmentButton(eEquipmentType::Potion)
{
}

PotionButton::~PotionButton()
{
}

void PotionButton::Update()
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
			if (item->GetItemType() == mType)
			{
				mItem = item;
				mbDrop = true;
				item->SetSlotInventory(this);
				item->SetDrop(mbDrop);
			}
			else
			{
				mbDrop = false;

				if (item->GetSlotInventory() == this)
					item->SetSlotInventory(nullptr);

				item->SetDrop(mbDrop);
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
