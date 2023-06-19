#include "CToolScene.h"
#include "CTilePallet.h"
#include "CCamera.h"
#include "CCameraScript.h"
#include "CObject.h"
#include "CRenderer.h"
#include "CMaterial.h"

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
		mTilePallet->SetToolScene(this);
	}

	{
		/*TileObject* object = Object::Instantiate<TileObject>(eLayerType::Tile, this);

		Transform* objectTr = object->GetComponent<Transform>();


		objectTr->SetPosition(Vector3(5000.f, 5000.f, 50.f));
		objectTr->SetSize(Vector3(400.f, 200.f, 1.0f));
		object->SetScreenIndex(0, 0);*/
	}

	{
		/*float sizeX = 400.f;
		float sizeY = 200.f;
		for (int y = 0; y < 40; ++y)
		{
			for (int x = 0; x < 40; ++x)
			{
				int Xpos = (x - y) * TILE_X_HALF_SIZE;
				int Ypos = (x + y) * TILE_Y_HALF_SIZE;

				TileObject* tile = Object::Instantiate<TileObject>(eLayerType::Tile, this);
				Transform* tr = tile->GetComponent<Transform>();
				UINT maxX = tile->GetMaterial()->GetTexture(eTextureSlot::T0).lock()->GetMaxX();
				UINT maxY = tile->GetMaterial()->GetTexture(eTextureSlot::T0).lock()->GetMaxY();

				tile->SetMaxIndex(maxX, maxY);
				tile->SetIndex(3, 30);

				tile->SetScreenIndex(x, y);

				tr->SetPosition(Vector3(5000.f + Xpos, 5000.f + Ypos, 50.f));
				tr->SetSize(Vector3(sizeX, sizeY, 1.0f));
			}
		}*/

		//	for (int y = 0; y < 20; ++y)
		//	{
		//		for (int x = 0; x < 20; ++x)
		//		{
		//			if (x + y >= 20)
		//				continue;

		//			TileObject* tile = Object::Instantiate<TileObject>(eLayerType::Tile, this);
		//			Transform* tr = tile->GetComponent<Transform>();
		//			UINT maxX = tile->GetMaterial()->GetTexture(eTextureSlot::T0).lock()->GetMaxX();
		//			UINT maxY = tile->GetMaterial()->GetTexture(eTextureSlot::T0).lock()->GetMaxY();

		//			tile->SetMaxIndex(maxX, maxY);
		//			tile->SetIndex(3, 30);


		//			tr->SetPosition(Vector3((- 1 * x) * (sizeX * 0.5f) + 5000.f, (y* sizeY) + (x * (sizeY * 0.5f)) + 5000.f, 50.0f));
		//			tr->SetSize(Vector3(sizeX, sizeY, 1.0f));

		//			tile->SetScreenIndex(x + indexX, y + indexY);
		//		}
		//	}
		//}

		//{
		//	WallObject* Wall = Object::Instantiate<WallObject>(eLayerType::Wall, this);

		//	std::weak_ptr<Texture2D> tex = ResourceManager::GetInstance()->Find<Texture2D>(L"Wall_1Object");
		//	Wall->SetTexture(tex);

		//	int Xpos = (1 - 1) * TILE_X_HALF_SIZE;
		//	int Ypos = (1 + 1) * TILE_Y_HALF_SIZE;

		//	Transform* tr = Wall->GetComponent<Transform>();
		//	tr->SetPosition(Vector3(5000.f + Xpos, 5000.f + Ypos, 49.f));
		//	Wall->SetOffset(Vector2(0.0f, 200.f));
		//	Wall->SetWidthHeigth(Vector2(600.f, 600.f));

		//	Wall->SetScrrenIndex(0 + 1, 0 + 1);
		//}

		//{
		//	WallObject* Wall = Object::Instantiate<WallObject>(eLayerType::Wall, this);

		//	int Xpos = ((-1 + 1) - (-1 + 1)) * TILE_X_HALF_SIZE;
		//	int Ypos = ((1 + -1) + (1 + -1)) * TILE_Y_HALF_SIZE;

		//	std::weak_ptr<Texture2D> tex = ResourceManager::GetInstance()->Find<Texture2D>(L"Wall_4Object");
		//	Wall->SetTexture(tex);

		//	Transform* tr = Wall->GetComponent<Transform>();
		//	tr->SetPosition(Vector3(5000.f + Xpos, 5000.f + Ypos, 49.f));
		//	Wall->SetOffset(Vector2(0.0f, 250.f));
		//	Wall->SetWidthHeigth(Vector2(150.f, 600.f));

		//	Wall->SetScrrenIndex(-1 + 1, -1 + 1);
		//}

		//{
		//	int x = 1;
		//	int y = 12;
		//	for (int i = 0; i < 11; ++i)
		//	{
		//		WallObject* Wall = Object::Instantiate<WallObject>(eLayerType::Wall, this);

		//		int Xpos = ((x + i + 1) - (y + 1)) * TILE_X_HALF_SIZE;
		//		int Ypos = (x + 1 + i + y + 1) * TILE_Y_HALF_SIZE;

		//		std::weak_ptr<Texture2D> tex = ResourceManager::GetInstance()->Find<Texture2D>(L"Wall_9Object");
		//		Wall->SetTexture(tex);

		//		Transform* tr = Wall->GetComponent<Transform>();
		//		tr->SetPosition(Vector3(5000.f + Xpos, 5000.f + Ypos, 49.f));
		//		Wall->SetOffset(Vector2(60.0f, -90.f));
		//		Wall->SetWidthHeigth(Vector2(300.f, 500.f));

		//		Wall->SetScrrenIndex(x + i + 1, y + 1);
		//	}
		//}

		//{
		//	WallObject* Wall = Object::Instantiate<WallObject>(eLayerType::Wall, this);

		//	int Xpos = ((12 + 1) - (1 + 1)) * TILE_X_HALF_SIZE;
		//	int Ypos = ((12 + 1) + (1 + 1)) * TILE_Y_HALF_SIZE;

		//	std::weak_ptr<Texture2D> tex = ResourceManager::GetInstance()->Find<Texture2D>(L"Wall_5Object");
		//	Wall->SetTexture(tex);

		//	Transform* tr = Wall->GetComponent<Transform>();
		//	tr->SetPosition(Vector3(5000.f + Xpos, 5000.f + Ypos, 49.f));
		//	Wall->SetOffset(Vector2(-100.f, -100.f));
		//	Wall->SetWidthHeigth(Vector2(300.f, 600.f));

		//	Wall->SetScrrenIndex(12 + 1, 1 + 1);
		//}

		//{
		//	int x = 12;
		//	int y = 2;
		//	for (int i = 0; i < 10; ++i)
		//	{
		//		WallObject* Wall = Object::Instantiate<WallObject>(eLayerType::Wall, this);

		//		int Xpos = ((x + 1) - (i + y + 1)) * TILE_X_HALF_SIZE;
		//		int Ypos = ((x + 1) + (i + y + 1)) * TILE_Y_HALF_SIZE;

		//		std::weak_ptr<Texture2D> tex = ResourceManager::GetInstance()->Find<Texture2D>(L"Wall_53Object");
		//		Wall->SetTexture(tex);

		//		Transform* tr = Wall->GetComponent<Transform>();
		//		tr->SetPosition(Vector3(5000.f + Xpos, 5000.f + Ypos, 49.f));
		//		Wall->SetOffset(Vector2(-110.f, -130.f));
		//		Wall->SetWidthHeigth(Vector2(300.f, 600.f));

		//		Wall->SetScrrenIndex(x + 1, i + y + 1);
		//	}
		//}

		//{
		//	WallObject* Wall = Object::Instantiate<WallObject>(eLayerType::Wall, this);

		//	int Xpos = ((12 + 1) - (12 + 1)) * TILE_X_HALF_SIZE;
		//	int Ypos = ((12 + 1) + (12 + 1)) * TILE_Y_HALF_SIZE;

		//	std::weak_ptr<Texture2D> tex = ResourceManager::GetInstance()->Find<Texture2D>(L"Wall_4Object");
		//	Wall->SetTexture(tex);

		//	Transform* tr = Wall->GetComponent<Transform>();
		//	tr->SetPosition(Vector3(5000.f + Xpos, 5000.f + Ypos, 49.f));
		//	Wall->SetOffset(Vector2(-20.0f, -190.f));
		//	Wall->SetWidthHeigth(Vector2(180.f, 650.f));

		//	Wall->SetScrrenIndex(12 + 1, 12 + 1);
		//}

		//{
		//	for (int i = 0; i < 10; ++i)
		//	{
		//		////300, 500
		//		////180, -65
		//		////y 1 , x 2
		//		int x = 2;
		//		int y = 1;

		//		WallObject* Wall = Object::Instantiate<WallObject>(eLayerType::Wall, this);

		//		std::weak_ptr<Texture2D> tex = ResourceManager::GetInstance()->Find<Texture2D>(L"Wall_1Object");
		//		Wall->SetTexture(tex);

		//		int Xpos = ((x + i + 1) - (y + 1)) * TILE_X_HALF_SIZE;
		//		int Ypos = (x + 1 + i + y + 1) * TILE_Y_HALF_SIZE;


		//		Transform* tr = Wall->GetComponent<Transform>();
		//		tr->SetPosition(Vector3(5000.f + Xpos, 5000.f + Ypos, 49.f));
		//		Wall->SetOffset(Vector2(180.f, -65.f));
		//		Wall->SetWidthHeigth(Vector2(300.f, 500.f));

		//		Wall->SetTexture(ResourceManager::GetInstance()->Find<Texture2D>(L"Wall_54Object"));

		//		Wall->SetScrrenIndex(x + i + 1, y + 1);
		//	}

		//	for (int i = 0; i < 10; ++i)
		//	{
		//		////300, 500
		//		////180, -65
		//		////y 1 , x 2
		//		int x = 1;
		//		int y = 2;

		//		WallObject* Wall = Object::Instantiate<WallObject>(eLayerType::Wall, this);

		//		std::weak_ptr<Texture2D> tex = ResourceManager::GetInstance()->Find<Texture2D>(L"Wall_1Object");
		//		Wall->SetTexture(tex);

		//		int Xpos = ((x + 1) - (i + y + 1)) * TILE_X_HALF_SIZE;
		//		int Ypos = ((x + 1) + (i + y + 1)) * TILE_Y_HALF_SIZE;


		//		Transform* tr = Wall->GetComponent<Transform>();
		//		tr->SetPosition(Vector3(5000.f + Xpos, 5000.f + Ypos, 49.f));
		//		Wall->SetOffset(Vector2(-220.f, -70.f));
		//		Wall->SetWidthHeigth(Vector2(300.f, 500.f));

		//		Wall->SetTexture(ResourceManager::GetInstance()->Find<Texture2D>(L"Wall_53Object"));

		//		Wall->SetScrrenIndex(x + 1, y + i + 1);
		//	}
		//}
	}
}

void ToolScene::Update()
{
	if (mTilePallet == nullptr)
		return;

	if (Input::GetInstance()->GetKeyDown(eKeyCode::F_1))
	{
		mTilePallet->Save();
		return;
	}
	else if (Input::GetInstance()->GetKeyDown(eKeyCode::F_2))
	{
		mTilePallet->Load();
		return;
	}

	if (Input::GetInstance()->GetKeyDown(eKeyCode::UP))
	{
		int modenum = (int)mode + 1;
		if (modenum >= (UINT)eToolRenderMode::END)
		{
			mode = eToolRenderMode::ALL;
		}
		else
		{
			mode = (eToolRenderMode)modenum;
		}

	}
	else if (Input::GetInstance()->GetKeyDown(eKeyCode::DOWN))
	{
		int modenum = (int)mode - 1;
		if (modenum < (int)eToolRenderMode::ALL)
		{
			mode = eToolRenderMode::Unmovable_Area;
		}
		else
		{
			mode = (eToolRenderMode)modenum;
		}
	}


	switch (mode)
	{
	case eToolRenderMode::ALL:
	{
		const vector<GameObject*>& objects = mLayers[(UINT)eLayerType::Wall].GetGameObjects();
		for (GameObject* obj : objects)
		{
			WallObject* wall = dynamic_cast<WallObject*>(obj);
			if (wall == nullptr)
				continue;

			wall->SetAlpha(1.0f);
		}

		break;
	}
	case eToolRenderMode::TILE:
	{
		const vector<GameObject*>& objects = mLayers[(UINT)eLayerType::Wall].GetGameObjects();
		for (GameObject* obj : objects)
		{
			WallObject* wall = dynamic_cast<WallObject*>(obj);
			if (wall == nullptr)
				continue;

			wall->SetAlpha(0.6f);
		}
		break;
	}
	case eToolRenderMode::OBJECT:
	{
		const vector<GameObject*>& objects = mLayers[(UINT)eLayerType::Wall].GetGameObjects();
		for (GameObject* obj : objects)
		{
			WallObject* wall = dynamic_cast<WallObject*>(obj);
			if (wall == nullptr)
				continue;

			wall->SetAlpha(1.0f);
		}

		break;
	}
	case eToolRenderMode::Unmovable_Area:
	{
		const vector<GameObject*>& objects = mLayers[(UINT)eLayerType::Wall].GetGameObjects();
		for (GameObject* obj : objects)
		{
			WallObject* wall = dynamic_cast<WallObject*>(obj);
			if (wall == nullptr)
				continue;

			wall->SetAlpha(0.6f);
		}
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

	for (Layer& layer : mLayers)
	{
		if (mode == eToolRenderMode::TILE || mode == eToolRenderMode::Unmovable_Area)
		{
			if (layer.GetLayerType() == eLayerType::Wall)
				continue;
		}

		layer.Update();
	}
}

void ToolScene::FixedUpdate()
{
	if (mTilePallet == nullptr)
		return;

	Scene::FixedUpdate();
}

void ToolScene::Render()
{
	if (mTilePallet == nullptr)
		return;

	Scene::Render();
}

void ToolScene::OnEnter()
{
	Renderer::mainCamera = GetMainCam();
}
