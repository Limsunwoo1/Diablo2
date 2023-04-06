#include "CTileObject.h"
#include "CMeshRenderer.h"
#include "CComputeShader.h"
#include "CResourceManager.h"

TileObject::TileObject()
	: GameObject()
{
}

TileObject::~TileObject()
{
}

void TileObject::Initalize()
{
}

void TileObject::Update()
{
	GameObject::Update();
}

void TileObject::FixedUpdate()
{
	MeshRenderer* mr = GetComponent<MeshRenderer>();
	std::shared_ptr<Material> mater = mr->GetMaterial();
	mater->SetTexture(eTextureSlot::T0, ResourceManager::GetInstance()->Find<Texture2D>(L"ShopIdle"));

	GameObject::FixedUpdate();
}

void TileObject::Render()
{
	GameObject::Render();
}
