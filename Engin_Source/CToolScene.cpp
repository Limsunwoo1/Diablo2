#include "CToolScene.h"
#include "CTilePallet.h"
#include "CCamera.h"
#include "CCameraScript.h"
#include "CObject.h"
#include "CRenderer.h"

#include "CTileObject.h"
#include "CLight.h"

#include "CInput.h"
#include "CWallObject.h"
#include "CResourceManager.h"

ToolScene::ToolScene()
	: Scene(eSceneType::Tool)
	, mTilePallet(nullptr)
	, mode(eToolRenderMode::ALL)
{
}

ToolScene::~ToolScene()
{
	
}

void ToolScene::Initalize()
{

	{
		GameObject* directionalLight = Object::Instantiate<GameObject>(eLayerType::None, this);
		Transform* tr = directionalLight->GetComponent<Transform>();
		tr->SetPosition(Vector3(0.0f, 0.0f, -100.f));
		Light* lightcomp = directionalLight->AddComponent<Light>();
		lightcomp->SetType(eLightType::Directional);
		lightcomp->SetDiffuse(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
	}

	// mainCamera
	{
		GameObject* cameraObj = Object::Instantiate<GameObject>(eLayerType::Camera, this);
		Camera* camera = cameraObj->AddComponent<Camera>();
		cameraObj->AddComponent<CameraScript>();

		cameraObj->GetComponent<Transform>()->SetPosition(Vector3(5500.0f, 5500.0f, 1.0f));
		cameraObj->SetName(L"ToolMainCamera");

		camera->SetProjectionType(Camera::eProjectionType::Orthographic);

		mMainCamera = camera;
		Renderer::mainCamera = camera;
	}

	{
		mTilePallet = Object::Instantiate<TilePallet>(eLayerType::TilePllet, this);
	}

	{
		TileObject* object = Object::Instantiate<TileObject>(eLayerType::Tile, this);

		Transform* objectTr = object->GetComponent<Transform>();


		objectTr->SetPosition(Vector3(5000.f, 5000.f, 50.f));
		objectTr->SetSize(Vector3(400.f, 200.f, 1.0f));
	}

	int indexX = 0;
	int indexY = 0;
	{
		bool a = false;
		float sizeX = 400.f;
		float sizeY = 200.f;
		for (int y = 0; y < 20; ++y)
		{
			for (int x = 0; x < 20; ++x)
			{
				if (x + y >= 20)
					continue;

				TileObject* tile = Object::Instantiate<TileObject>(eLayerType::Tile, this);
				Transform* tr = tile->GetComponent<Transform>();
				UINT maxX = tile->GetMaterial()->GetTexture(eTextureSlot::T0).lock()->GetMaxX();
				UINT maxY = tile->GetMaterial()->GetTexture(eTextureSlot::T0).lock()->GetMaxY();

				tile->SetMaxIndex(maxX, maxY);
				tile->SetIndex(1, 3);

				tr->SetPosition(Vector3((x*(sizeX * 0.5f) + 5000.f), (y* sizeY) + (x * (sizeY * 0.5f)) + 5000.f, 50.0f));
				tr->SetSize(Vector3(sizeX, sizeY, 1.0f));

				indexX = x;
				indexY = y;

				tile->SetScreenIndex(x, y);

			}
		}

		for (int y = 0; y < 20; ++y)
		{
			for (int x = 0; x < 20; ++x)
			{
				if (x + y >= 20)
					continue;

				TileObject* tile = Object::Instantiate<TileObject>(eLayerType::Tile, this);
				Transform* tr = tile->GetComponent<Transform>();
				UINT maxX = tile->GetMaterial()->GetTexture(eTextureSlot::T0).lock()->GetMaxX();
				UINT maxY = tile->GetMaterial()->GetTexture(eTextureSlot::T0).lock()->GetMaxY();

				tile->SetMaxIndex(maxX, maxY);
				tile->SetIndex(1, 3);


				tr->SetPosition(Vector3((- 1 * x) * (sizeX * 0.5f) + 5000.f, (y* sizeY) + (x * (sizeY * 0.5f)) + 5000.f, 50.0f));
				tr->SetSize(Vector3(sizeX, sizeY, 1.0f));

				tile->SetScreenIndex(x + indexX, y + indexY);
			}
		}
	}

	{
		WallObject* Wall = Object::Instantiate<WallObject>(eLayerType::Wall, this);

		std::weak_ptr<Texture2D> tex = ResourceManager::GetInstance()->Find<Texture2D>(L"Wall_1Object");
		Wall->SetTexture(tex);

		Transform* tr = Wall->GetComponent<Transform>();
		tr->SetPosition(Vector3(5000.f, 5000.f, 49.f));
		Wall->SetOffset(Vector2(0.0f, 200.f));
		Wall->SetWidthHeigth(Vector2(600.f, 600.f));
	}

	{
		WallObject* Wall = Object::Instantiate<WallObject>(eLayerType::Wall, this);

		int Xpos = (-1 - -1) * TILE_X_HALF_SIZE;
		int Ypos = (-1 + -1) * TILE_Y_HALF_SIZE;

		std::weak_ptr<Texture2D> tex = ResourceManager::GetInstance()->Find<Texture2D>(L"Wall_4Object");
		Wall->SetTexture(tex);

		Transform* tr = Wall->GetComponent<Transform>();
		tr->SetPosition(Vector3(5000.f + Xpos, 5000.f + Ypos, 49.f));
		Wall->SetOffset(Vector2(0.0f, 250.f));
		Wall->SetWidthHeigth(Vector2(150.f, 600.f));
	}

	{
		for (int i = 0; i < 10; ++i)
		{
			// 300, 500
			// 180, -65
			// y 1 , x 2
			int x = 2;
			int y = 1;

			WallObject* Wall = Object::Instantiate<WallObject>(eLayerType::Wall, this);

			std::weak_ptr<Texture2D> tex = ResourceManager::GetInstance()->Find<Texture2D>(L"Wall_1Object");
			Wall->SetTexture(tex);

			int Xpos = ((x + i) - y) * TILE_X_HALF_SIZE;
			int Ypos = (x + i + y )* TILE_Y_HALF_SIZE;


			Transform* tr = Wall->GetComponent<Transform>();
			tr->SetPosition(Vector3(5000.f + Xpos  , 5000.f + Ypos, 49.f));
			Wall->SetOffset(Vector2(180.f, -65.f));
			Wall->SetWidthHeigth(Vector2(300.f, 500.f));

			Wall->SetTexture(ResourceManager::GetInstance()->Find<Texture2D>(L"Wall_54Object"));
		}

		for (int i = 0; i < 10; ++i)
		{
			// 300, 500
			// 180, -65
			// y 1 , x 2
			int x = 1;
			int y = 2;

			WallObject* Wall = Object::Instantiate<WallObject>(eLayerType::Wall, this);

			std::weak_ptr<Texture2D> tex = ResourceManager::GetInstance()->Find<Texture2D>(L"Wall_1Object");
			Wall->SetTexture(tex);

			int Xpos = (x - (i + y)) * TILE_X_HALF_SIZE;
			int Ypos = (x + (i + y)) * TILE_Y_HALF_SIZE;


			Transform* tr = Wall->GetComponent<Transform>();
			tr->SetPosition(Vector3(5000.f + Xpos, 5000.f + Ypos, 49.f));
			Wall->SetOffset(Vector2(-220.f, -60.f));
			Wall->SetWidthHeigth(Vector2(300.f, 500.f));

			Wall->SetTexture(ResourceManager::GetInstance()->Find<Texture2D>(L"Wall_53Object"));
		}
	}
}

void ToolScene::Update()
{
	if (mTilePallet == nullptr)
		return;

	if (Input::GetInstance()->GetKeyDown(eKeyCode::F_1))		mTilePallet->Save();
	else if (Input::GetInstance()->GetKeyDown(eKeyCode::F_2))	mTilePallet->Load();

	if (Input::GetInstance()->GetKeyDown(eKeyCode::M))
	{
		UINT modenum = (UINT)mode + 1;
		mode = (eToolRenderMode)modenum;
	}


	switch (mode)
	{
	case eToolRenderMode::ALL:
	{
		mMainCamera->EnableLayerMasks();

		const vector<GameObject*>& wallobjects = mLayers[(UINT)eLayerType::Wall].GetGameObjects();
		for (GameObject* obj : wallobjects)
		{
			obj->Active();
		}

		const vector<GameObject*>& tileobjects = mLayers[(UINT)eLayerType::Tile].GetGameObjects();
		for (GameObject* obj : tileobjects)
		{
			obj->Active();
		}

		break;
	}
	case eToolRenderMode::TILE: 
	{
		mMainCamera->EnableLayerMasks();
		mMainCamera->TurnLayerMask(eLayerType::Wall, false);

		const vector<GameObject*>& tileobjects = mLayers[(UINT)eLayerType::Tile].GetGameObjects();
		for (GameObject* obj : tileobjects)
		{
			obj->Active();
		}

		const vector<GameObject*>& objects = mLayers[(UINT)eLayerType::Wall].GetGameObjects();
		for (GameObject* obj : objects)
		{
			obj->Paused();
		}
		break;
	}
	case eToolRenderMode::OBJECT:
	{
		mMainCamera->EnableLayerMasks();
		mMainCamera->TurnLayerMask(eLayerType::Tile, false);

		const vector<GameObject*>& wallobjects = mLayers[(UINT)eLayerType::Wall].GetGameObjects();
		for (GameObject* obj : wallobjects)
		{
			obj->Active();
		}

		const vector<GameObject*>& objects = mLayers[(UINT)eLayerType::Tile].GetGameObjects();
		for (GameObject* obj : objects)
		{
			obj->Paused();
		}

		break;
	}
	case eToolRenderMode::OFF:
	{
		mMainCamera->DisableLayerMasks();
		break;
	}
	case eToolRenderMode::END: 
	{
		mode = eToolRenderMode::ALL;
	}
	break;
	default:
		break;
	}

	if (mode == eToolRenderMode::OFF)
		return;

	Scene::Update();
	mTilePallet->Update();
}

void ToolScene::FixedUpdate()
{
	if (mTilePallet == nullptr)
		return;

	mTilePallet->FixedUpdate();

	Scene::FixedUpdate();
}

void ToolScene::Render()
{
	if (mTilePallet == nullptr)
		return;

	mTilePallet->Render();

	Scene::Render();
}

void ToolScene::OnEnter()
{
	Renderer::mainCamera = GetMainCam();
}
