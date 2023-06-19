#include "CTileObject.h"
#include "CMeshRenderer.h"
#include "CComputeShader.h"
#include "CResourceManager.h"
#include "CRenderer.h"
#include "CConstantBuffer.h"
#include "CInput.h"
#include "CSceneManager.h"
#include "CObject.h"
#include "CToolScene.h"
#include "CTilePallet.h"

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
	, mTileType(0)
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
	mArr.resize(4);

	Transform* Tr = GetComponent<Transform>();
	Tr->SetSize(Vector3(TILE_X_HALF_SIZE * 2.f, TILE_Y_HALF_SIZE * 2.f, 1.0f));
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

	Vector2 mousePos = Input::GetInstance()->GetMouseWorldPos(Renderer::mainCamera);

	if (_Editor.GetActive())
		mousePos = _Editor.GetEditorWorldMousePos();

	if (tilePos.x - (tileScale.x * 0.5f) > mousePos.x || tilePos.x + (tileScale.x * 0.5f) < mousePos.x)
		return;

	if (tilePos.y - (tileScale.y * 0.5f) > mousePos.y || tilePos.y + (tileScale.y * 0.5f) < mousePos.y)
		return;

	ToolScene* tool = dynamic_cast<ToolScene*>(SceneManager::GetInstance()->GetScene(eSceneType::Tool));
	if (PickTile(mousePos, eTilePickType::TileALL))
	{
		if (Input::GetInstance()->GetKeyPress(eKeyCode::LCTRL))
		{
			if (_Editor.GetActive() && Input::GetInstance()->GetKeyPress(eKeyCode::LBTN) && tool != nullptr)
			{
				if (tool->GetToolRenderMode() == eToolRenderMode::TILE)
				{
					mIndexX = _Editor.GetTileIndexX();
					mIndexY = _Editor.GetTileIndexY();
				}
				mbOnTile = true;
			}
			else if (_Editor.GetActive() && Input::GetInstance()->GetKeyPress(eKeyCode::RBTN) && tool != nullptr)
			{
				if (tool->GetToolRenderMode() == eToolRenderMode::TILE)
				{
					Object::ObjectDestroy(this);
				}
				mbOnTile = true;
			}
		}

		if (Input::GetInstance()->GetKeyPress(eKeyCode::LBTN) && _Editor.GetActive() && tool != nullptr)
		{
			if (tool->GetToolRenderMode() == eToolRenderMode::TILE)
			{
				Renderer::InspectorGameObject = this;

				gui::Inspector* inspector = _Editor.GetWidget<gui::Inspector>("Inspector");
				inspector->SetTargetGameObject(Renderer::InspectorGameObject);
				inspector->InitalizeTargetGameObject();
			}
		}

		if (_Editor.GetActive()) // 에디터모드에서 메모리할당 제한
		{
			_Editor.GetWidget<gui::Game>("Game")->SetCreateTile(false);
		}

		mbOnTile = true;


		// 4등분 타일 충돌체크
		if		((PickTile(mousePos, eTilePickType::Tile0))) { mArrIdx = 0; }
		else if ((PickTile(mousePos, eTilePickType::Tile1))) { mArrIdx = 1; }
		else if ((PickTile(mousePos, eTilePickType::Tile2))) { mArrIdx = 2; }
		else if ((PickTile(mousePos, eTilePickType::Tile3))) { mArrIdx = 3; }

		// 툴모드일때 CarveObject 생성
		if (mArrIdx >= 0
			&& tool != nullptr
			&& tool->GetToolRenderMode() == eToolRenderMode::Unmovable_Area)
		{
			TilePallet* pallet = tool->GetTilePallet();
			if (pallet == nullptr)
				return;

			if (Input::GetInstance()->GetKeyPress(eKeyCode::LBTN))
			{
				mArr[mArrIdx] = 1;
				pallet->InserCarveObject(mScreenIndexX, mScreenIndexY, mArrIdx, this);
			}
			else if (Input::GetInstance()->GetKeyPress(eKeyCode::RBTN))
			{
				mArr[mArrIdx] = 0;
				pallet->DeleteCarveObject(mScreenIndexX, mScreenIndexY, mArrIdx);
			}

			SetPassSituation();
		}

		// PlayGame
		if (mArrIdx >= 0 && tool == nullptr)
		{

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
		info.TileType = mTileType;

		ConstantBuffer* cb = Renderer::constantBuffers[(UINT)eCBType::TileData];
		cb->SetData(&info);
		cb->Bind(eShaderStage::PS);
	}

	mbOnTile = false;
	mArrIdx = -1;

	GameObject::Render();
}

void TileObject::SetPassSituation()
{
	if (mArr[0] == 1 && mArr[3] == 1)
	{
		mbPass = false;
	}

	if (mArr[1] == 1 && mArr[2] == 1)
	{
		mbPass = false;
	}
}

bool TileObject::PickTile(Vector2 mousePos, eTilePickType type)
{
	Transform* tileTr = GetComponent<Transform>();
	Vector3 tilePos = tileTr->GetPosition();
	Vector3 tileScale = tileTr->GetScale() * tileTr->GetSize();

	// 기울기
	float fslope = (tileScale.y * 0.5f) / (tileScale.x * 0.5f);
	float fSlope[4] =
	{
		fslope,
		-fslope,
		fslope,
		-fslope,
	};
	Vector3 InScale = tileScale * 0.25f;

	vector<Vector2> vVertex = {};
	vVertex.resize(4);

	if (type == eTilePickType::TileALL)		InScale = tileScale * 0.5f;
	else if (type == eTilePickType::Tile0)	tilePos.y -= InScale.y;
	else if (type == eTilePickType::Tile1)	tilePos.x += InScale.x;
	else if (type == eTilePickType::Tile2)	tilePos.x -= InScale.x;
	else if (type == eTilePickType::Tile3)	tilePos.y += InScale.y;

	vVertex = GetIsometricVertex(tilePos, InScale);

	// 절편
	float fY_Intercept[4] =
	{
		vVertex[0].y - (fSlope[0] * vVertex[0].x),
		vVertex[1].y - (fSlope[1] * vVertex[1].x),
		vVertex[2].y - (fSlope[2] * vVertex[2].x),
		vVertex[3].y - (fSlope[3] * vVertex[3].x),
	};

	//0 = ax + b - y;
	if (0 < fSlope[0] * mousePos.x + fY_Intercept[0] - mousePos.y &&
		0 < fSlope[1] * mousePos.x + fY_Intercept[1] - mousePos.y &&
		0 > fSlope[2] * mousePos.x + fY_Intercept[2] - mousePos.y &&
		0 > fSlope[3] * mousePos.x + fY_Intercept[3] - mousePos.y)
	{
		return true;
	}
	return false;
}

Vector2 TileObject::TileCarvePos(eTilePickType type)
{
	Transform* tileTr = GetComponent<Transform>();
	Vector3 tilePos = tileTr->GetPosition();
	Vector3 tileScale = tileTr->GetScale() * tileTr->GetSize();

	Vector3 InScale = tileScale * 0.25f;

	if (type == eTilePickType::Tile0)		tilePos.y -= InScale.y;
	else if (type == eTilePickType::Tile1)	tilePos.x += InScale.x;
	else if (type == eTilePickType::Tile2)	tilePos.x -= InScale.x;
	else if (type == eTilePickType::Tile3)	tilePos.y += InScale.y;
	else return Vector2(-1.f, -1.f);

	return Vector2(tilePos.x, tilePos.y);
}

vector<Vector2> TileObject::GetIsometricVertex(Vector3 pos, Vector3 size)
{
	vector<Vector2> vVerTex = {};
	vVerTex.resize(4);

	vVerTex[0] = Vector2(pos.x, pos.y + size.y);
	vVerTex[1] = Vector2(pos.x + size.x, pos.y);
	vVerTex[2] = Vector2(pos.x, pos.y - size.y);
	vVerTex[3] = Vector2(pos.x - size.x, pos.y);

	return vVerTex;
}