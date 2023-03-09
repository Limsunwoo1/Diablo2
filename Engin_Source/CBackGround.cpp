#include "CBackGround.h"
#include "CResourceManager.h"
#include "CMeshRenderer.h"
#include "CMaterial.h"

BackGround::BackGround()
	: GameObject()
	, mTextuer2D(nullptr)
	, mbMode(eRenderingMode::Opaque)
{
}

BackGround::~BackGround()
{
}

void BackGround::Initalize()
{
	
}

void BackGround::Update()
{
	GameObject::Update();
}

void BackGround::FixedUpdate()
{
	//MeshRenderer* mr = GetComponent<MeshRenderer>();
	//mr->GetMaterial()->SetTexture(mTextuer2D);
	GameObject::FixedUpdate();
}

void BackGround::Render()
{
	MeshRenderer* mr = GetComponent<MeshRenderer>();
	mr->GetMaterial()->SetTexture(mTextuer2D);
	mr->GetMaterial()->SetRenderingMode(mbMode);

	GameObject::Render();
}

std::shared_ptr<Texture2D> BackGround::FindTextuer2D(const std::wstring& name, const std::wstring& path)
{
	mTextuer2D = ResourceManager::GetInstance()->Find<Texture2D>(name);

	if (mTextuer2D == nullptr)
	{
		mTextuer2D = std::make_shared<Texture2D>();
		mTextuer2D->Load(path);
		ResourceManager::GetInstance()->Insert<Texture2D>(name, mTextuer2D);
	}

	return mTextuer2D;
}