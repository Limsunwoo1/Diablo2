#include "CTileObject.h"
#include "CMeshRenderer.h"
#include "CComputeShader.h"
#include "CResourceManager.h"
#include "CRenderer.h"
#include "CConstantBuffer.h"

TileObject::TileObject()
	: GameObject()
	, mUV(Vector2::One)
	, mbPass(true)
	, mArr{}
	, mTexPath(L"")
{
	SetName(L"Tile");
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
	std::weak_ptr<Material> material = ResourceManager::GetInstance()->Find<Material>(L"TileMaterial");
	mr->SetMaterial(material);
	std::weak_ptr<Texture2D> tex = ResourceManager::GetInstance()->Find<Texture2D>(L"TestTile");
	material.lock()->SetTexture(eTextureSlot::T0,tex);


	GameObject::FixedUpdate();
}

void TileObject::Render()
{
	if (mMaxX != 0 && mMaxY != 0)
	{
		Renderer::TileDataCB info = {};

		float indexX = mIndexX;
		float indexY = mIndexY;

		float MaxX = mMaxX;
		float MaxY = mMaxY;

		info.StartUV = Math::Vector2(indexX / MaxX, indexY / MaxY);
		info.EndUV = Math::Vector2(((indexX + 1) / MaxX), ((indexY + 1) / MaxY));

		ConstantBuffer* cb = Renderer::constantBuffers[(UINT)eCBType::TileData];
		cb->SetData(&info);
		cb->Bind(eShaderStage::PS);
	}

	GameObject::Render();
}
