#include "CLavaTile.h"
#include "CResourceManager.h"
#include "CBaseRenderer.h"
#include "CAnimator.h"
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

LavaTile::LavaTile()
	: TileObject()
{
	for (int i = 0; i < mArr.size(); ++i)
	{
		mArr[i] = 1;
	}

	std::weak_ptr<Material> mater = ResourceManager::GetInstance()->Find<Material>(L"LavaMaterial");
	mMaterial = mater.lock().get();

	Animator* animator = AddComponent<Animator>();
	std::weak_ptr<Texture2D> tex = ResourceManager::GetInstance()->Find<Texture2D>(L"LavaTile");
	animator->Create(L"LavaTile", tex, Vector2::Zero, Vector2(160.f, 80.f), Vector2::Zero, Vector2(160.f * 4, 80.f),4, 0.4f);
	animator->Play(L"LavaTile");
	
	Transform* Tr = GetComponent<Transform>();
	Tr->SetSize(Vector3(TILE_X_HALF_SIZE * 2.f, TILE_Y_HALF_SIZE * 2.f, 1.0f));

	mTileType = 1;
}

LavaTile::~LavaTile()
{
}



extern gui::Editor _Editor;

void LavaTile::Update()
{
	GetComponent<Animator>()->Update();
	TileObject::Update();
}
