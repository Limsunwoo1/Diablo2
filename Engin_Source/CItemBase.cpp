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

	// �����ۿ� ���콺�� �浹 üũ
	if (mousePos.x > colPos.x + (colScale.x * 0.5f) || mousePos.x < colPos.x - (colScale.x * 0.5f))
		return;
	if (mousePos.y > colPos.y + (colScale.x * 0.5f) || mousePos.y < colPos.y - (colScale.y * 0.5f))
		return;

	if (Input::GetInstance()->GetKeyDown(eKeyCode::LBTN))
	{
		SetStage(false);

		// ���콺�� �ٸ� �������� ���� �ʰ� �ִ� ����
		if (Input::GetInstance()->GetPickItem() == nullptr)
		{
			mbPick = true;
			Input::GetInstance()->SetPickItem(this);

			// ���ĭ�� ���� �κ��丮���� �Ǻ��Ͽ�
			// �˸��� ��ġ�� ������ش�
			if (mSlotInventory == nullptr && mInventory != nullptr)
				mInventory->ClearPocketSlot(this);
			else if (mSlotInventory != nullptr)
				mSlotInventory->ClearPocketSlot(this);
			else
				int a = 0;
		}
		// ���콺�� �������� �����ٰ� ���� ����
		else if(mbDrop) // �������� ������ �ִ� ��Ȳ���� üũ �����̳� �κ��丮����
						// üũ���ش�
		{
			mbPick = false;
			Input::GetInstance()->SetPickItem(nullptr);

			// ���ĭ�� ���� �κ��丮���� �Ǻ��Ͽ�
			// �˸��� ��ġ�� ������ش�
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

	// ���� �ȵ� �������̸� ��Ȳ�� �´� ���� �����
	info.pick = mbPick;
	// ���� �κ��丮 �������� �����ε� �Ǻ��� ���ǿ��´�
	// �ִϸ��̼����
	info.stage = mbStage;

	if (mInventory) // ������ �ִ»�Ȳ (�ʷ�)
		info.drop = mInventory->GetDrop();
	else			// ������ ��Ȳ (����)
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

	// ��ŷ�� �������� �κ��丮�迭 ���ΰ� �ƴ� �ٸ������ִٸ�
	// ĵ������ ��������ʰ� �þ߸� ���� ���İ��� ���ϰ� ���
	info.OnInventory = mbOnInvnetory;

	ConstantBuffer* cb = Renderer::constantBuffers[(UINT)eCBType::ItemData];
	cb->SetData(&info);
	cb->Bind(eShaderStage::ALL);

	GameObject::Render();
}

