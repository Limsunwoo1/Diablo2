#include "CPanel.h"
#include "CResourceManager.h"
#include "CInput.h"
#include "CTransform.h"


Panel::Panel(eUIType type)
	: UiBase(type)
{

}
Panel::~Panel()
{
	int a = 0;
}

void Panel::OnInit()
{
}

void Panel::OnActive()
{
	mbEnable = true;

	for (UiBase* child : mChilds)
	{
		if (child == nullptr)
			continue;

		child->Active();
	}
}

void Panel::UnActive()
{
	mbEnable = false;

	for (UiBase* child : mChilds)
	{
		if (child == nullptr)
			continue;

		child->UnActive();
	}
}

void Panel::OnInActive()
{
}

void Panel::OnUpdate()
{
}

void Panel::OnRender(HDC hdc)
{
	
}

void Panel::OnClear()
{
}

void Panel::Initalize()
{
}

void Panel::Update()
{
	UiBase::Update();
}

void Panel::FixedUpdate()
{
	UiBase::FixedUpdate();
}

void Panel::Render()
{
	MeshRenderer* mr = GetComponent<MeshRenderer>();
	if (mbEnable == false)
	{
		mr->SetRenderStop(true);
		return;
	}
	else
	{
		mr->SetRenderStop(false);
	}

	UiBase::Render();
}

void Panel::InitAnimation()
{

}

void Panel::InitRenderer(const wstring& materialName, const wstring& textureName, const std::wstring& texturepath)
{
	MeshRenderer* mr = AddComponent<MeshRenderer>();
	weak_ptr<Mesh> mesh = ResourceManager::GetInstance()->Find<Mesh>(L"RectMesh");
	weak_ptr<Material> material = ResourceManager::GetInstance()->Find<Material>(materialName);
	weak_ptr<Texture2D> tex = ImageLoad(textureName, texturepath);

	material.lock()->SetTexture(eTextureSlot::T0, tex);
	mr->SetMesh(mesh);
	mr->SetMaterial(material);
}