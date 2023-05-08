#pragma once
#include "CTitleScene.h"
#include "CRenderer.h"
#include "CMeshRenderer.h"
#include "CSpriteRenderer.h"
#include "CResourceManager.h"
#include "CTexture2D.h"
#include "CPlayerScript.h"
#include "CCameraScript.h"
#include "CGridScript.h"
#include "CObject.h"
#include "CInput.h"
#include "CCollider2D.h"
#include "CCollisionManager.h"
#include "CBackGround.h"
#include "CApplication.h"
#include "CAnimator.h"
#include "CLight.h"

extern CApplication Application;


TitleScene::TitleScene()
	: Scene(eSceneType::Title)
{

}
TitleScene::~TitleScene()
{

}

void TitleScene::Initalize()
{
	// Light
	{
		GameObject* directionalLight = Object::Instantiate<GameObject>(eLayerType::Player);
		Transform* tr = directionalLight->GetComponent<Transform>();
		tr->SetPosition(Vector3(0.0f, 0.0f, -100.f));
		Light* lightcomp = directionalLight->AddComponent<Light>();
		lightcomp->SetType(eLightType::Directional);
		lightcomp->SetDiffuse(Vector4(1.f, 1.0f, 1.f, 1.0f));
	}
	
	{
		// Main Camera Game Object
		GameObject* cameraObj = Object::Instantiate<GameObject>(eLayerType::Camera);
		Camera* cameraComp = cameraObj->AddComponent<Camera>();
		//cameraComp->RegisterCameraInRenderer();
		//cameraComp->TurnLayerMask(eLayerType::UI, false);
		cameraObj->AddComponent<CameraScript>();
		cameraComp->SetProjectionType(Camera::eProjectionType::Orthographic);
		Renderer::mainCamera = cameraComp;
	}

	RECT winRect;
	GetClientRect(Application.GetHwnd(), &winRect);

	float width = (float)(winRect.right - winRect.left);
	float height = (float)(winRect.bottom - winRect.top);

	//renderer::cameras[0] = cameraComp;
	// BackGround
	{
		BackGround* ground = Object::Instantiate<BackGround>(eLayerType::BackGround);
		ground->SetName(L"BackGround");
		
		Transform* tr = ground->GetComponent<Transform>();
		tr->SetScale(Vector3(8.0f, 6.0f, 0.0f));

		std::weak_ptr<Texture2D> tex = ResourceManager::GetInstance()->Load<Texture2D>(L"Title1", L"UI//Title_01.png");
		ground->SetTexture(tex);

		MeshRenderer* mr = ground->AddComponent<MeshRenderer>();

		std::shared_ptr<Mesh> mesh = ResourceManager::GetInstance()->Find<Mesh>(L"FadeMesh");
		std::shared_ptr<Material> backmaterial = ResourceManager::GetInstance()->Find<Material>(L"RectMaterial");

		mr->SetMesh(mesh);
		mr->SetMaterial(backmaterial);
	}
	
	Scene::Initalize();
}

void TitleScene::Update()
{
	Scene::Update();

	if (Input::GetInstance()->GetKeyDown(eKeyCode::LBTN))
	{
		SceneManager::GetInstance()->LoadScene(eSceneType::MainTitle);
	}
}

void TitleScene::FixedUpdate()
{
	Scene::FixedUpdate();
}

void TitleScene::Render()
{
	Scene::Render();
}

void TitleScene::OnEnter()
{

}

void TitleScene::OnExit()
{

}