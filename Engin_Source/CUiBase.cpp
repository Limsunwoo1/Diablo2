#include "CUiBase.h"
#include "CResourceManager.h"
#include "CTexture2D.h"



UiBase::UiBase(eUIType type)
	: mType(type)
	, mbFullScreen(false)
	, mbEnable(false)
{

}

UiBase::~UiBase()
{

}

void UiBase::Initialize()
{
	OnInit();
}

void UiBase::Active()
{
	mbEnable = true;
	OnActive();
}

void UiBase::InActive()
{
	mbEnable = false;
	OnInActive();
}

void UiBase::Tick()
{
	if (mbEnable == false)
		return;

	OnTick();
}

void UiBase::Render(HDC hdc)
{
	if (mbEnable == false)
		return;

	OnRender(hdc);
}

void UiBase::UIClear()
{
	OnClear();
}

void UiBase::ImageLoad(const std::wstring& key, const std::wstring& path)
{
	shared_ptr<Texture2D> tex = ResourceManager::GetInstance()->Load<Texture2D>(key, path);

	SetSize(Vector2(tex.get()->GetWidth(), tex.get()->GetHeight()));
}

void UiBase::SetChild(Vector2 vector, UiBase* child)
{
	child->SetParent(this);
	mChilds.push_back(make_pair(vector, child));
}
void UiBase::DeleteChild(UiBase* child)
{
	std::vector<pair<Vector2, UiBase*>>::iterator iter;
	iter = mChilds.begin();
	for (; iter != mChilds.end(); ++iter)
	{
		if (iter->second == child)
		{
			mChilds.erase(iter);
			return;
		}
	}
}
