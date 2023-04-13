#include "CPanel.h"
#include "CResourceManager.h"


Panel::Panel(eUIType type)
	: UiBase(type)
{

}
Panel::~Panel()
{

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
	UiBase::Render();
}

void Panel::InitAnimation()
{

}

void Panel::InitRenderer(const wstring& materialName, const wstring& textureName, const std::wstring& texturepath)
{
	MeshRenderer* mr = AddComponent<MeshRenderer>();
	shared_ptr<Mesh> mesh = ResourceManager::GetInstance()->Find<Mesh>(L"RectMesh");
	shared_ptr<Material> material = ResourceManager::GetInstance()->Find<Material>(materialName);
	shared_ptr<Texture2D> tex = ImageLoad(textureName, texturepath);

	material->SetTexture(eTextureSlot::T0, tex);
	mr->SetMesh(mesh);
	mr->SetMaterial(material);
}