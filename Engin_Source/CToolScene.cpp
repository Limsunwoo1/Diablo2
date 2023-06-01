#include "CToolScene.h"
#include "CTilePallet.h"
#include "CCamera.h"
#include "CCameraScript.h"
#include "CObject.h"
#include "CRenderer.h"

#include "CTileObject.h"
#include "CLight.h"

#include "CInput.h"

ToolScene::ToolScene()
	: Scene(eSceneType::Tool)
	, mTilePallet(nullptr)
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

	TileObject* object = Object::Instantiate<TileObject>(eLayerType::Tile, this);

	Transform* objectTr = object->GetComponent<Transform>();


	objectTr->SetPosition(Vector3(5000.f, 5000.f, 50.f));
	objectTr->SetSize(Vector3(200.f, 100.f, 1.0f));
}

void ToolScene::Update()
{
	if (mTilePallet == nullptr)
		return;

	if (Input::GetInstance()->GetKeyDown(eKeyCode::F_1))		mTilePallet->Save();
	else if (Input::GetInstance()->GetKeyDown(eKeyCode::F_2))	mTilePallet->Load();

	mTilePallet->Update();

	Scene::Update();
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
