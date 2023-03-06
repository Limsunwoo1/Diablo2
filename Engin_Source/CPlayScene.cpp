#include "CPlayScene.h"
#include "CCamera.h"
#include "CCameraScript.h"
#include "CObject.h"
#include "CInput.h"

PlayScene::PlayScene()
	: Scene(eSceneType::Play)
{

}
PlayScene::~PlayScene()
{

}

void PlayScene::Initalize()
{
	GameObject* cameraObj = Object::Instantiate<GameObject>(eLayerType::Camera, this);
	Camera* cameraComp = cameraObj->AddComponent<Camera>();
	//cameraComp->RegisterCameraInRenderer();
	cameraComp->TurnLayerMask(eLayerType::UI, false);
	cameraObj->AddComponent<CameraScript>();

	Scene::Initalize();
}

void PlayScene::Update()
{
	Scene::Update();

	if (Input::GetInstance()->GetKeyDown(eKeyCode::N))
	{
		SceneManager::GetInstance()->LoadScene(eSceneType::Title);
	}
}

void PlayScene::FixedUpdate()
{
	Scene::FixedUpdate();
}

void PlayScene::Render()
{
	Scene::Render();
}

void PlayScene::OnEnter()
{

}

void PlayScene::OnExit()
{

}