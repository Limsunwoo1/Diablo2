#include "CTileObject.h"
#include "CMeshRenderer.h"
#include "CComputeShader.h"
#include "CResourceManager.h"

TileObject::TileObject()
	: GameObject()
	, mSize(Vector2::Zero)
	, mUV(Vector2::One)
	, mPalletIndex(-1)
	, mbPass(true)
	, mArr{}
	, mTexPath(L"")
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
	std::weak_ptr<Material> mater = mr->GetMaterial();
	mater.lock()->SetTexture(eTextureSlot::T0, ResourceManager::GetInstance()->Find<Texture2D>(L"ShopIdle"));

	GameObject::FixedUpdate();
}

void TileObject::Render()
{
	GameObject::Render();
}
