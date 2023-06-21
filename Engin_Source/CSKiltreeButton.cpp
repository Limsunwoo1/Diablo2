#include "CSKiltreeButton.h"
#include "CInput.h"
#include "CTransform.h"
#include "CCollider2D.h"
#include "CSpriteRenderer.h"
#include "CResourceManager.h"

#include "..//Dx11_Engine/GuiEditor.h"


#include <iostream>

extern gui::Editor _Editor;

SkiltreeButton::SkiltreeButton()
	: Button(eUIType::Button)
{
	Transform* Tr = GetComponent<Transform>();
	Tr->SetSize(Vector3(80.f, 80.f, 1.0f));
}

SkiltreeButton::~SkiltreeButton()
{
	UiBase* child = GetChild();
	SetChild(nullptr);
	if (child)
		delete child;

}

void SkiltreeButton::InitTexture(std::weak_ptr<Texture2D> tex)
{
	SpriteRenderer* sr = AddComponent<SpriteRenderer>();

	std::weak_ptr<Mesh> mesh = ResourceManager::GetInstance()->Find<Mesh>(L"RectMesh");
	std::weak_ptr<Shader> shader = ResourceManager::GetInstance()->Find<Shader>(L"SpriteShader");


	mMaterial = std::make_shared<Material>();
	mMaterial->SetRenderingMode(graphics::eRenderingMode::Transparent);
	mMaterial->SetShader(shader);
	mMaterial->SetTexture(eTextureSlot::T0, tex);

	sr->SetMesh(mesh);
	sr->SetMaterial(mMaterial);
}

void SkiltreeButton::Initalize()
{
}

void SkiltreeButton::Update()
{
	if (mbEnable == false)
		return;

	GameObject::Update();

	if (mPoint >= mMaxPoint && GetChild())
		GetChild()->Update();

	Vector2 mousePos = Input::GetInstance()->GetMouseWorldPos(false);

	if (_Editor.GetActive())
	{
		mousePos = _Editor.GetEditorUiMousePos();
	}

	Transform* tr = GetComponent<Transform>();
	float x, y, z;
	x = tr->GetWorldMatrix()._41;
	y = tr->GetWorldMatrix()._42;
	z = tr->GetWorldMatrix()._43;
	Vector3 colPos{ x,y,z };
	Vector3 collpos = tr->GetPosition();
	Vector3 colScale = tr->GetScale() * tr->GetSize();

	// 마우스가 버튼이랑 충돌 X
	SetPointToRect(0);

	if (mousePos.x > collpos.x + (colScale.x * 0.5f) || mousePos.x < collpos.x - (colScale.x * 0.5f))
		return;
	if (mousePos.y > collpos.y + (colScale.y * 0.5f) || mousePos.y < collpos.y - (colScale.y * 0.5f))
		return;

	// 마우스가 버튼이랑 충돌 0
	SetPointToRect(1);

	if (Input::GetInstance()->GetKeyUp(eKeyCode::LBTN))
		UpPoint();
}

void SkiltreeButton::FixedUpdate()
{
	if (mbEnable == false)
		return;

	GameObject::FixedUpdate();

	if (mPoint >= mMaxPoint && GetChild())
		GetChild()->FixedUpdate();
}

void SkiltreeButton::Render()
{
	if (mbEnable == false)
		return;

	GameObject::Render();

	if (mPoint >= mMaxPoint && GetChild())
		GetChild()->Render();
}

