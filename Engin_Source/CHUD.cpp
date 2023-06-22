#include "CHUD.h"


HUD::HUD(eUIType type)
	: UiBase(type)
	, mbMouseOn(false)
{

}
HUD::~HUD()
{

}

void HUD::OnInit()
{
}

void HUD::OnActive()
{
}

void HUD::OnInActive()
{
}

void HUD::OnUpdate()
{
	//for (int i = 0; i < mChilds.size(); ++i)
	//{
	//	if (mChilds[i] == nullptr)
	//		continue;

	//	UiBase* ui = mChilds[i];
	//	//ui->SetPos(GetPos() + mChilds[i].first);
	//}
}

void HUD::OnRender(HDC hdc)
{
	if (mImage == nullptr)
		return;


}

void HUD::OnClear()
{
}

void HUD::Initalize()
{
}

void HUD::Update()
{
	UiBase::Update();
}

void HUD::FixedUpdate()
{
	UiBase::FixedUpdate();
}

void HUD::Render()
{
	UiBase::Render();
}

void HUD::InitAnimation()
{
}

void HUD::InitRenderer(const wstring& materialName, const wstring& textureName, const std::wstring& textruepath)
{
}
