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

	if (mbPick && !mbStage)
		tr->SetPosition(Vector3(mousePos.x, mousePos.y, 1.0f));

	if (mousePos.x > colPos.x + (colScale.x * 0.5f) || mousePos.x < colPos.x - (colScale.x * 0.5f))
		return;
	if (mousePos.y > colPos.y + (colScale.x * 0.5f) || mousePos.y < colPos.y - (colScale.y * 0.5f))
		return;

	if (Input::GetInstance()->GetKeyDown(eKeyCode::LBTN))
	{
		if (Input::GetInstance()->GetPickItem() == nullptr)
		{
			mbPick = true;
			Input::GetInstance()->SetPickItem(this);

			mInventory->ClearPocketSlot(this);
		}
		else if(mInventory->GetDrop())
		{
			mbPick = false;
			Input::GetInstance()->SetPickItem(nullptr);
			mInventory->DropItem(this);
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
	info.pick = mbPick;
	info.stage = mbStage;
	info.drop = mInventory->GetDrop();

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

	ConstantBuffer* cb = Renderer::constantBuffers[(UINT)eCBType::ItemData];
	cb->SetData(&info);
	cb->Bind(eShaderStage::ALL);

	GameObject::Render();
}

