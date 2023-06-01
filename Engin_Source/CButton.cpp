#include "CButton.h"
#include "CInput.h"
#include "CTransform.h"
#include "CCollider2D.h"

#include "..//Dx11_Engine/GuiEditor.h"


#include <iostream>

extern gui::Editor _Editor;

Button::Button(eUIType type)
	: UiBase(type)
	, mbMouseOn(false)
{
	mOnClick = std::bind(&Button::Click, this);

	Collider2D* col = this->AddComponent<Collider2D>();
	col->SetType(eColliderType::Rect);
}
Button::~Button()
{

}

void Button::OnInit()
{
}

void Button::OnActive()
{
	mbEnable = true;

	for (UiBase* child : mChilds)
	{
		if (child == nullptr)
			continue;

		child->OnActive();
	}
}

void Button::UnActive()
{
	mbEnable = false;

	for (UiBase* child : mChilds)
	{
		if (child == nullptr)
			continue;

		child->UnActive();
	}
}

void Button::OnUpdate()
{
	
}

void Button::OnRender(HDC hdc)
{

}

void Button::OnClear()
{
}

void Button::Initalize()
{
	mOnClick = std::bind(&Button::Click, this);
}

void Button::Update()
{
	GameObject::Update();

	Vector2 mousePos = Input::GetInstance()->GetMouseWorldPos(false);

	if (!_Editor.GetActive())
	{
		mousePos = _Editor.GetEditorUiMousePos();
	}

	std::cout << mousePos.x << "  X ��ǥ  " << mousePos.y << "  Y ��ǥ" << std::endl;

	Transform* tr = GetComponent<Transform>();
	float x, y, z;
	x = tr->GetWorldMatrix()._41;
	y = tr->GetWorldMatrix()._42;
	z = tr->GetWorldMatrix()._43;
	Vector3 colPos{ x,y,z };
	Vector3 collpos = tr->GetPosition();
	Vector3 colScale = tr->GetScale() * tr->GetSize();

	// ���콺�� ��ư�̶� �浹 X
	SetPointToRect(0);

	if (mousePos.x > collpos.x + (colScale.x * 0.5f) || mousePos.x < collpos.x - (colScale.x * 0.5f))
		return;
	if (mousePos.y > collpos.y + (colScale.y * 0.5f) || mousePos.y < collpos.y - (colScale.y * 0.5f))
		return;

	// ���콺�� ��ư�̶� �浹 0
	SetPointToRect(1);
}

void Button::FixedUpdate()
{
	GameObject::FixedUpdate();
}

void Button::Render()
{
	GameObject::Render();
}

void Button::InitAnimation()
{
}

void Button::InitRenderer(const wstring& materialName, const wstring& textureName, const std::wstring& textruepath)
{
}


void Button::Click()
{
	int a = 0;
}

void Button::BindEvnet(std::function<void()> fun)
{
	mOnClick = fun;
}

void Button::OnEvent()
{
	mOnClick();
}
