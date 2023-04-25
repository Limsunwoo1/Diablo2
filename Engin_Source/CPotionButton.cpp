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
				mbDrop = false;

				if (item->GetSlotInventory() == this)
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
				mItem = nullptr;
				item->SetSlotInventory(nullptr);
				item->SetDrop(false);
			}
		}
	}
	else
	{
		// 아이템이 없으면 투명하게 렌더링
		mbOnRender = false;
	}
}
