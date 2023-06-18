#include "CPanel.h"
#include "CResourceManager.h"
#include "CInput.h"
#include "CTransform.h"
#include "CWorldManager.h"
#include "Cplayer.h"

Panel::Panel(eUIType type)
	: UiBase(type)
	, mType(eGaugeUi::None)
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

	if (mType != eGaugeUi::None)
	{
		Player* player = dynamic_cast<Player*>(WorldManager::GetInstance()->GetPlayer());

		if (player == nullptr)
			return;

		Renderer::PlayerDataCB info = {};
		info.Gaugetype = mType;
		info.RunGauge = player->GetRunTime() / player->GetMaxRunTime();
		info.hpGauge = player->GetHP() / player->GetMaxHP();
		info.mpGauge = player->GetMP() / player->GetMaxMP();
		//info.hpGauge = ;

		ConstantBuffer* cb = Renderer::constantBuffers[(UINT)eCBType::PlayerData];
		cb->SetData(&info);

		cb->Bind(eShaderStage::ALL);
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