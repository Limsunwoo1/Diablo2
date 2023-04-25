#include "CItemBase.h"
#include "CInput.h"
#include "CCollider2D.h"
#include "CUIManager.h"
#include "CWorldManager.h"

ItemBase::ItemBase(eEquipmentType type)
	: GameObject()
	, mType(type)
	, mXSize(0)
	, mYSize(0)
	, mbStage(false)
	, mbPick(false)
	, mbOnInvnetory(false)
	, mMaterial(nullptr)
{
}

ItemBase::~ItemBase()
{
}

void ItemBase::Update()
{
	GameObject::Update();

	Vector2	mousePos = Input::GetInstance()->GetMouseWorldPos(mbStage);

	Transform* tr = GetComponent<Transform>();
	Vector3 colPos = tr->GetPosition();
	Vector3 colScale = tr->GetScale();

	// 아이템와 마우스의 충돌 체크
	if (mousePos.x > colPos.x + (colScale.x * 0.5f) || mousePos.x < colPos.x - (colScale.x * 0.5f))
		return;
	if (mousePos.y > colPos.y + (colScale.x * 0.5f) || mousePos.y < colPos.y - (colScale.y * 0.5f))
		return;

	if (Input::GetInstance()->GetKeyDown(eKeyCode::LBTN))
	{
		SetStage(false);

		// 마우스가 다른 아이템을 집지 않고 있는 상태
		if (Input::GetInstance()->GetPickItem() == nullptr)
		{
			mbPick = true;
			Input::GetInstance()->SetPickItem(this);

			// 장비칸위 인지 인벤토리인지 판별하여
			// 알맞은 위치에 드롭해준다
			if (mSlotInventory == nullptr && mInventory != nullptr)
				mInventory->ClearPocketSlot(this);
			else if (mSlotInventory != nullptr)
				mSlotInventory->ClearPocketSlot(this);
			else
				int a = 0;
		}
		// 마우스가 아이템을 집었다가 놓는 상태
		else if(mbDrop) // 아이템을 놓을수 있는 상황인지 체크 슬롯이나 인벤토리에서
						// 체크해준다
		{
			mbPick = false;
			Input::GetInstance()->SetPickItem(nullptr);

			// 장비칸위 인지 인벤토리인지 판별하여
			// 알맞은 위치에 드롭해준다
			if (mSlotInventory == nullptr && mInventory != nullptr)
				mInventory->DropItem(this);
			else if (mSlotInventory != nullptr)
				mSlotInventory->DropItem(this);
		}
		else
		{
			UiBase* mainPanel = UIManager::GetInstance()->GetUiInstance<UiBase>(L"mainPanel");
			UiBase* InventoryPanel = UIManager::GetInstance()->GetUiInstance<UiBase>(L"mainInventory");

			Transform* mainTr = mainPanel->GetComponent<Transform>();
			Vector3 mainPos = mainTr->GetPosition();
			Vector3 mainScale = mainTr->GetScale();

			Transform* InvenTr = InventoryPanel->GetComponent<Transform>();
			Vector3 InvenPos = InvenTr->GetPosition();
			Vector3 InvenScale = InvenTr->GetScale();

			Transform* ItemTr = this->GetComponent<Transform>();
			Vector3 ItemPos = ItemTr->GetPosition();
			Vector3 ItemScale = ItemTr->GetScale();

			if (mainPos.x - (mainScale.x * 0.5f) >= ItemPos.x && mainPos.x + (mainScale.x * 0.5f) <= ItemPos.x
				&& mainPos.y - (mainScale.y * 0.5f) <= ItemPos.y && mainPos.y + (mainScale.y * 0.5f) >= ItemPos.y)
				return;

			if (InvenPos.x - (ItemScale.x * 0.5f) >= ItemPos.x && InvenPos.x + (ItemScale.x * 0.5f) <= ItemPos.x
				&& InvenPos.y - (ItemScale.y * 0.5f) <= ItemPos.y && InvenPos.y + (ItemScale.y * 0.5f) >= ItemPos.y)
				return;

			GameObject* player = WorldManager::GetInstance()->GetPlayer();
			if (!player)
				return;

			Transform* playerTr = player->GetComponent<Transform>();
			Vector3 playerPos = playerTr->GetPosition();
			
			ItemPos = playerPos;
			ItemTr->SetPosition(ItemPos);

			mbPick = false;
			Input::GetInstance()->SetPickItem(nullptr);

			if(GetInventory())
				GetInventory()->DeleteItem(this);
			if(GetSlotInventory())
				GetSlotInventory()->DeleteItem(this);

			this->SetInventory(nullptr);
			this->SetSlotInventory(nullptr);

			SetStage(true);
		}
	}

	Input::GetInstance()->SetMouseItemPick(mbPick);
}

void ItemBase::FixedUpdate()
{
	GameObject::FixedUpdate();
}

void ItemBase::Render()
{
	Renderer::ItemDataCB info = {};
	Renderer::ItemDataCB* clear = nullptr;

	// 내가 픽된 아이템이면 상황에 맞는 알파 값출력
	info.pick = mbPick;
	// 내가 인벤토리 내부인지 월드인데 판별후 조건에맞는
	// 애니메이션출력
	info.stage = mbStage;

	if (mInventory) // 놓을수 있는상황 (초록)
		info.drop = mInventory->GetDrop();
	else			// 못놓는 상황 (빨강)
		info.drop = false;
	

	Vector4 color = Vector4::One;
	if (info.pick)
	{
		color.w = 0.5f;

		if (info.drop)
		{
			color *= Vector4(0.2f, 1.0f, 0.2f, 0.5f);
		}
		else
		{
			color *= Vector4(1.0f, 0.2f, 0.2f, 0.5f);
		}
	}
	info.canversColor = color;

	// 피킹된 아이템이 인벤토리배열 내부가 아닌 다른곳에있다면
	// 캔버스를 출력하지않고 시야를 위해 알파값을 연하게 출력
	info.OnInventory = mbOnInvnetory;

	ConstantBuffer* cb = Renderer::constantBuffers[(UINT)eCBType::ItemData];
	cb->SetData(&info);
	cb->Bind(eShaderStage::ALL);

	GameObject::Render();
}

