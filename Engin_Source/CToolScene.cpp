#include "CToolScene.h"
#include "CTilePallet.h"
#include "CCamera.h"
#include "CCameraScript.h"
#include "CObject.h"
#include "CRenderer.h"

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
	// mainCamera
	{
		GameObject* cameraObj = Object::Instantiate<GameObject>(eLayerType::Camera, this);
		Camera* camera = cameraObj->AddComponent<Camera>();
		cameraObj->AddComponent<CameraScript>();

		cameraObj->GetComponent<Transform>()->SetRotation(Vector3(0.0f, 0.0f, 0.f));
		cameraObj->GetComponent<Transform>()->SetPosition(Vector3(0.0f, 0.0f, 0.0f));
		cameraObj->SetName(L"ToolMainCamera");

		camera->SetProjectionType(Camera::eProjectionType::Prespective);

		mMainCamera = camera;
		Renderer::mainCamera = camera;
	}

	{
		mTilePallet = Object::Instantiate<TilePallet>(eLayerType::TilePllet, this);
	}
}

void ToolScene::Update()
{
	if (mTilePallet == nullptr)
		return;

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
