#include "CPanel.h"

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
}

void Panel::OnInActive()
{
}

void Panel::OnTick()
{
	for (int i = 0; i < mChilds.size(); ++i)
	{
		if (mChilds[i].second == nullptr)
			continue;

		UiBase* ui = mChilds[i].second;
		ui->SetPos(GetPos() + mChilds[i].first);
	}
}

void Panel::OnRender(HDC hdc)
{
	
}

void Panel::OnClear()
{
}

void Panel::SetChild(Vector2 vector, UiBase* child)
{
	child->SetParent(this);
	mChilds.push_back(make_pair(vector, child));
}