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
	for (UiBase* child : mChilds)
	{
		if (child == nullptr)
			continue;

		delete child;
		child = nullptr;
	}
}

void UiBase::Initalize()
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
	UnActive();
}

void UiBase::Update()
{
	if (mbEnable == false)
		return;

	GameObject::Update();

	for (UiBase* child : mChilds)
	{
		if (child == nullptr)
			continue;

		child->Update();
	}
}

void UiBase::FixedUpdate()
{
	GameObject::FixedUpdate();

	if (mParent)
	{
		Transform* parentTr = mParent->GetComponent<Transform>();
		Matrix& mat = parentTr->GetWorldMatrix();

		Transform* mTr = GetComponent<Transform>();
		Matrix& mWorld = mTr->GetWorldMatrix();

		Vector3 pos = Vector3(	mat._41 + mWorld._41, 
								mat._42 + mWorld._42, 
								mat._43 + mWorld._43 );
		mWorld._41 = pos.x;
		mWorld._42 = pos.y;
		mWorld._43 = pos.z;

		mTr->SetWorldMatrix(mWorld);
	}

	for (UiBase* child : mChilds)
	{
		if (child == nullptr)
			continue;

		child->FixedUpdate();
	}
}

void UiBase::Render()
{
	GameObject::Render();

	for (UiBase* child : mChilds)
	{
		if (child == nullptr)
			continue;

		child->Render();
	}
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

std::shared_ptr<Texture2D> UiBase::ImageLoad(const std::wstring& key, const std::wstring& path)
{
	shared_ptr<Texture2D> tex = ResourceManager::GetInstance()->Load<Texture2D>(key, path);

	return tex;
}

void UiBase::SetChild(UiBase* child)
{
	child->SetParent(this);
	mChilds.push_back(child);
}

void UiBase::DeleteChild(UiBase* child)
{
	std::vector<UiBase*>::iterator iter;
	iter = mChilds.begin();
	for (; iter != mChilds.end(); ++iter)
	{
		if (*iter == child)
		{
			mChilds.erase(iter);

			delete child;
			child = nullptr;
			return;
		}
	}
}
