#include "CCurSelectSkilButton.h"
#include "CUIManager.h"
#include "CSelectButtonSystem.h"
#include "CResourceManager.h"
#include "CSpriteRenderer.h"
#include "CSkilSelectPanel.h"
#include "CInput.h"

#include "CTransform.h"
#include "CCollider2D.h"

#include "..//Dx11_Engine/GuiEditor.h"
#include "CUiBase.h"


#include <iostream>

extern gui::Editor _Editor;


CurSelectSkilButton::CurSelectSkilButton()
	: Button(eUIType::Button)
	//,  mType(eSkilList::Attack)
{
	BindEvnet(std::bind(&CurSelectSkilButton::OpenSelectPanel, this));
	
	std::weak_ptr<Texture2D> tex = ResourceManager::GetInstance()->Load<Texture2D>(L"AttackButton", L"..//Resource//UI//AttackUi.png");

	mMaterial = std::make_shared<Material>();
	std::weak_ptr<Mesh> mesh = ResourceManager::GetInstance()->Find<Mesh>(L"RectMesh");
	std::weak_ptr<Shader> shader= ResourceManager::GetInstance()->Find<Shader>(L"SpriteShader");
	mMaterial->SetTexture(eTextureSlot::T0, tex);
	mMaterial->SetShader(shader);
	mMaterial->SetRenderingMode(graphics::eRenderingMode::Transparent);

	SpriteRenderer* sr = AddComponent<SpriteRenderer>();
	sr->SetMesh(mesh);
	sr->SetMaterial(mMaterial);
	
}

CurSelectSkilButton::~CurSelectSkilButton()
{
}

void CurSelectSkilButton::InItTex(std::weak_ptr<Texture2D> tex)
{
	mMaterial->SetTexture(eTextureSlot::T0, tex);
}

void CurSelectSkilButton::Update()
{
	GameObject::Update();

	if (mbChildRun && mChild)
		mChild->Update();

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
		OpenSelectPanel();
}

void CurSelectSkilButton::OpenSelectPanel()
{
	UiBase* inven = UIManager::GetInstance()->GetUiInstance<UiBase>(L"mainInventory");
	UiBase* skiltree = UIManager::GetInstance()->GetUiInstance<UiBase>(L"SkilTree");

	inven->UnActive();
	skiltree->UnActive();

	SkilSelectPanel* panel = UIManager::GetInstance()->GetUiInstance<SkilSelectPanel>(L"SkilSelect");
	if(panel->GetIsAble())
		panel->UnActive();
	else
		panel->OnActive();
}
