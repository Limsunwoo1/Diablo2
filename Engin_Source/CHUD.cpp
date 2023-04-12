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

void HUD::OnTick()
{
	for (int i = 0; i < mChilds.size(); ++i)
	{
		if (mChilds[i].second == nullptr)
			continue;

		UiBase* ui = mChilds[i].second;
		ui->SetPos(GetPos() + mChilds[i].first);
	}
}

void HUD::OnRender(HDC hdc)
{
	if (mImage == nullptr)
		return;


}

void HUD::OnClear()
{
}
