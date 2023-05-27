#include "CTileObject.h"
#include "CMeshRenderer.h"
#include "CComputeShader.h"
#include "CResourceManager.h"
#include "CRenderer.h"
#include "CConstantBuffer.h"
#include "CInput.h"


#include <iostream>
TileObject::TileObject()
	: GameObject()
	, mUV(Vector2::One)
	, mbPass(true)
	, mArr{}
	, mTexPath(L"")
	, mMaxX(1)
	, mMaxY(1)
	, mIndexX(1)
	, mIndexY(1)
{
	SetName(L"Tile");

	MeshRenderer* mr = AddComponent<MeshRenderer>();
	std::weak_ptr<Mesh> mesh = ResourceManager::GetInstance()->Find<Mesh>(L"RectMesh");
	std::weak_ptr<Material> material = ResourceManager::GetInstance()->Find<Material>(L"TileMaterial");
	mr->SetMesh(mesh);
	mr->SetMaterial(material);
	std::weak_ptr<Texture2D> tex = ResourceManager::GetInstance()->Find<Texture2D>(L"TestTile");
	material.lock()->SetTexture(eTextureSlot::T0, tex);

	mMaterial = material.lock().get();

	mMaxX = tex.lock()->GetMaxX();
	mMaxY = tex.lock()->GetMaxY();

	mbUpdate = false;
}

TileObject::~TileObject()
{
}

void TileObject::Initalize()
{
}

void TileObject::Update()
{
	Transform* tileTr = GetComponent<Transform>();
	Vector3 tilePos = tileTr->GetPosition();
	Vector3 tileScale = tileTr->GetScale() * tileTr->GetSize();

	Vector2 mousePos = Input::GetInstance()->GetMouseWorldPos(Renderer::IsometricCamera);

	mIndexX = 2;
	mIndexY = 2;

	if (tilePos.x - (tileScale.x * 0.5f) > mousePos.x || tilePos.x + (tileScale.x * 0.5f) < mousePos.x)
		return;

	if (tilePos.y - (tileScale.y * 0.5f) > mousePos.y || tilePos.y + (tileScale.y * 0.5f) < mousePos.y)
		return;

	// 기울기
	float fslope = (tileScale.y * 0.5f) / (tileScale.x * 0.5f);
	float fSlope[4] =
	{
		fslope,
		-fslope,
		fslope,
		-fslope,
	};

	// 마름모 정점
	Vector2 vVertex[4] =
	{
		{tilePos.x, tilePos.y + (tileScale.y * 0.5f)},
		{tilePos.x + (tileScale.x * 0.5f), tilePos.y},
		{ tilePos.x, tilePos.y - (tileScale.y * 0.5f)},
		{ tilePos.x - (tileScale.x * 0.5f), tilePos.y},
	};

	// 절편
	float fY_Intercept[4] =
	{
		vVertex[0].y - (fSlope[0] * vVertex[0].x),
		vVertex[1].y - (fSlope[1] * vVertex[1].x),
		vVertex[2].y - (fSlope[2] * vVertex[2].x),
		vVertex[3].y - (fSlope[3] * vVertex[3].x),
	};

	//0 = ax + b - y;

	float tset[4] =
	{
		fSlope[0] * mousePos.x + fY_Intercept[0] - mousePos.y,
		fSlope[1] * mousePos.x + fY_Intercept[1] - mousePos.y,
		fSlope[2] * mousePos.x + fY_Intercept[2] - mousePos.y,
		fSlope[3] * mousePos.x + fY_Intercept[3] - mousePos.y
	};

	if (0 < fSlope[0] * mousePos.x + fY_Intercept[0] - mousePos.y &&
		0 < fSlope[1] * mousePos.x + fY_Intercept[1] - mousePos.y &&
		0 > fSlope[2] * mousePos.x + fY_Intercept[2] - mousePos.y &&
		0 > fSlope[3] * mousePos.x + fY_Intercept[3] - mousePos.y)
	{
		mIndexX = 1;
		mIndexY = 37;
	}

	GameObject::Update();
}

void TileObject::FixedUpdate()
{
	if (mbUpdate == true)
		return;

	GameObject::FixedUpdate();

	mbUpdate = true;
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
