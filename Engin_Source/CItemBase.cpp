#include "CItemBase.h"
#include "CInput.h"

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

	if (Input::GetInstance()->GetKeyPress(eKeyCode::LBTN))
	{
		mbPick = true;

		Input::GetInstance()->SetPickItem(this);
	}
	else if (Input::GetInstance()->GetKeyUp(eKeyCode::LBTN))
	{
		mbPick = false;
	}

	Input::GetInstance()->SetMouseItemPick(mbPick);

	int a = 0;
}

void ItemBase::FixedUpdate()
{
	GameObject::FixedUpdate();
}

void ItemBase::Render()
{
	GameObject::Render();
}

