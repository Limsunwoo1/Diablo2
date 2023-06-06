#include "CTileObject.h"
#include "CMeshRenderer.h"
#include "CComputeShader.h"
#include "CResourceManager.h"
#include "CRenderer.h"
#include "CConstantBuffer.h"
#include "CInput.h"
#include "CSceneManager.h"
#include "CObject.h"

#include "..//Dx11_Engine/GuiEditor.h"
#include "..//Dx11_Engine/GuiGame.h"
#include "..//Dx11_Engine/GuiInspector.h"

extern gui::Editor _Editor;

TileObject::TileObject()
	: GameObject()
	, mUV(Vector2::One)
	, mbPass(true)
	, mbOnTile(false)
	, mArr{}
	, mArrIdx(-1)
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
	std::weak_ptr<Texture2D> tex = ResourceManager::GetInstance()->Find<Texture2D>(L"Tile");
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
	mbOnTile = false;
	mArrIdx = -1;

	Transform* tileTr = GetComponent<Transform>();
	Vector3 tilePos = tileTr->GetPosition();
	Vector3 tileScale = tileTr->GetScale() * tileTr->GetSize();

	Vector2 mousePos = Input::GetInstance()->GetMouseWorldPos(Renderer::mainCamera);

	if (_Editor.GetActive())
		mousePos = _Editor.GetEditorWorldMousePos();

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
		if (_Editor.GetActive() && Input::GetInstance()->GetKeyPress(eKeyCode::LBTN))
		{
			mIndexX = _Editor.GetTileIndexX();
			mIndexY = _Editor.GetTileIndexY();
			mbOnTile = true;
		}
		else if (_Editor.GetActive() && Input::GetInstance()->GetKeyPress(eKeyCode::RBTN))
		{
			Object::ObjectDestroy(this);
			mbOnTile = true;
		}
		else if (Input::GetInstance()->GetKeyDown(eKeyCode::F) && _Editor.GetActive())
		{
			Renderer::InspectorGameObject = this;

			gui::Inspector* inspector = _Editor.GetWidget<gui::Inspector>("Inspector");
			inspector->SetTargetGameObject(Renderer::InspectorGameObject);
			inspector->InitalizeTargetGameObject();
		}
		else if (_Editor.GetActive()) // 에디터모드에서 메모리할당 제한
		{
			mbOnTile = true;
		}

		_Editor.GetWidget<gui::Game>("Game")->SetCreateTile(false);


		// 기울기
		float childFslope = ((tileScale.y * 0.5f) / 4.f) / ((tileScale.x * 0.5f) / 4.f);
		float childfSlope[4] =
		{
			fslope,
			-fslope,
			fslope,
			-fslope,
		};

		// 마름모 정점
		Vector3 childPos = tilePos;
		childPos.y -= tileScale.y * 0.25f;
		Vector2 vVertex0[4] =
		{
			{childPos.x, childPos.y + (tileScale.y * 0.25f)},
			{childPos.x + (tileScale.x * 0.25f), childPos.y},
			{childPos.x, childPos.y - (tileScale.y * 0.25f)},
			{childPos.x - (tileScale.x * 0.25f), childPos.y},
		};

		childPos = tilePos;
		childPos.x += tileScale.x * 0.25f;
		Vector2 vVertex1[4] =
		{
			{childPos.x, childPos.y + (tileScale.y * 0.25f)},
			{childPos.x + (tileScale.x * 0.25f), childPos.y},
			{childPos.x, childPos.y - (tileScale.y * 0.25f)},
			{childPos.x - (tileScale.x * 0.25f), childPos.y},
		};

		childPos = tilePos;
		childPos.x -= tileScale.x * 0.25f;
		Vector2 vVertex2[4] =
		{
			{childPos.x, childPos.y + (tileScale.y * 0.25f)},
			{childPos.x + (tileScale.x * 0.25f), childPos.y},
			{childPos.x, childPos.y - (tileScale.y * 0.25f)},
			{childPos.x - (tileScale.x * 0.25f), childPos.y},
		};

		childPos = tilePos;
		childPos.y += tileScale.y * 0.25f;
		Vector2 vVertex3[4] =
		{
			{childPos.x, childPos.y + (tileScale.y * 0.25f)},
			{childPos.x + (tileScale.x * 0.25f), childPos.y},
			{childPos.x, childPos.y - (tileScale.y * 0.25f)},
			{childPos.x - (tileScale.x * 0.25f), childPos.y},
		};

		// 절편
		float fY_Intercept0[4] =
		{
			vVertex0[0].y - (childfSlope[0] * vVertex0[0].x),
			vVertex0[1].y - (childfSlope[1] * vVertex0[1].x),
			vVertex0[2].y - (childfSlope[2] * vVertex0[2].x),
			vVertex0[3].y - (childfSlope[3] * vVertex0[3].x),
		};

		float fY_Intercept1[4] =
		{
			vVertex1[0].y - (childfSlope[0] * vVertex1[0].x),
			vVertex1[1].y - (childfSlope[1] * vVertex1[1].x),
			vVertex1[2].y - (childfSlope[2] * vVertex1[2].x),
			vVertex1[3].y - (childfSlope[3] * vVertex1[3].x),
		};

		float fY_Intercept2[4] =
		{
			vVertex2[0].y - (childfSlope[0] * vVertex2[0].x),
			vVertex2[1].y - (childfSlope[1] * vVertex2[1].x),
			vVertex2[2].y - (childfSlope[2] * vVertex2[2].x),
			vVertex2[3].y - (childfSlope[3] * vVertex2[3].x),
		};

		float fY_Intercept3[4] =
		{
			vVertex3[0].y - (childfSlope[0] * vVertex3[0].x),
			vVertex3[1].y - (childfSlope[1] * vVertex3[1].x),
			vVertex3[2].y - (childfSlope[2] * vVertex3[2].x),
			vVertex3[3].y - (childfSlope[3] * vVertex3[3].x),
		};

		if (0 < childfSlope[0] * mousePos.x + fY_Intercept0[0] - mousePos.y &&
			0 < childfSlope[1] * mousePos.x + fY_Intercept0[1] - mousePos.y &&
			0 > childfSlope[2] * mousePos.x + fY_Intercept0[2] - mousePos.y &&
			0 > childfSlope[3] * mousePos.x + fY_Intercept0[3] - mousePos.y)
		{
			mArrIdx = 0;
		}
		else if (0 < childfSlope[0] * mousePos.x + fY_Intercept1[0] - mousePos.y &&
			0 < childfSlope[1] * mousePos.x + fY_Intercept1[1] - mousePos.y &&
			0 > childfSlope[2] * mousePos.x + fY_Intercept1[2] - mousePos.y &&
			0 > childfSlope[3] * mousePos.x + fY_Intercept1[3] - mousePos.y)
		{
			mArrIdx = 1;
		}
		else if (0 < childfSlope[0] * mousePos.x + fY_Intercept2[0] - mousePos.y &&
			0 < childfSlope[1] * mousePos.x + fY_Intercept2[1] - mousePos.y &&
			0 > childfSlope[2] * mousePos.x + fY_Intercept2[2] - mousePos.y &&
			0 > childfSlope[3] * mousePos.x + fY_Intercept2[3] - mousePos.y)
		{
			mArrIdx = 2;
		}
		else if (0 < childfSlope[0] * mousePos.x + fY_Intercept3[0] - mousePos.y &&
			0 < childfSlope[1] * mousePos.x + fY_Intercept3[1] - mousePos.y &&
			0 > childfSlope[2] * mousePos.x + fY_Intercept3[2] - mousePos.y &&
			0 > childfSlope[3] * mousePos.x + fY_Intercept3[3] - mousePos.y)
		{
			mArrIdx = 3;
		}
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
		info.OnTile = mbOnTile;
		info.ArrIdx = mArrIdx;

		ConstantBuffer* cb = Renderer::constantBuffers[(UINT)eCBType::TileData];
		cb->SetData(&info);
		cb->Bind(eShaderStage::PS);
	}

	GameObject::Render();
}
