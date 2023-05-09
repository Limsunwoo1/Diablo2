#include "CBackGround.h"
#include "CResourceManager.h"
#include "CMeshRenderer.h"
#include "CMaterial.h"

using namespace graphics;

BackGround::BackGround()
	: GameObject()
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

	if (mr)
	{
		mr->GetMaterial().lock()->SetRenderingMode(mbMode);
		if(mTexture.lock() != nullptr)
			mr->GetMaterial().lock()->SetTexture(eTextureSlot::T0, mTexture.lock());
	}

	GameObject::Render();
}