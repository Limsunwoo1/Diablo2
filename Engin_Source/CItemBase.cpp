#include "CItemBase.h"
#include "CInput.h"
#include "CCollider2D.h"

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
		// ���콺�� �������� ���� �ʰ� �ִ� ����
		if (Input::GetInstance()->GetPickItem() == nullptr)
		{
			mbPick = true;
			Input::GetInstance()->SetPickItem(this);

			// ���ĭ�� ���� �κ��丮���� �Ǻ��Ͽ�
			// �˸��� ��ġ�� ������ش�
			if (mSlotInventory == nullptr)
				mInventory->ClearPocketSlot(this);
			else
				mSlotInventory->ClearPocketSlot(this);
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
			else
				int a = 0; // ���忡 ���
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

