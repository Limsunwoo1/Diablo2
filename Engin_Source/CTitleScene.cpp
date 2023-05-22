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
		mMainCamera = cameraComp;

		cameraObj->GetComponent<Transform>()->SetPosition(Vector3(0.0f, 0.0f, -100.f));
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
		tr->SetSize(Vector3(1600.f, 900.f, 1.0f));

		tr->SetPosition(mMainCamera->GetOwner()->GetComponent<Transform>()->GetPosition());
		tr->SetPosition(Vector3(0.0f, 0.0f, 1.0f));

		std::weak_ptr<Texture2D> tex = ResourceManager::GetInstance()->Load<Texture2D>(L"Title1", L"UI//Title_01.png");
		ground->SetTexture(tex);

		MeshRenderer* mr = ground->AddComponent<MeshRenderer>();

		std::weak_ptr<Mesh> mesh = ResourceManager::GetInstance()->Find<Mesh>(L"RectMesh");
		std::weak_ptr<Material> backmaterial = ResourceManager::GetInstance()->Find<Material>(L"RectMaterial");

		mr->SetMesh(mesh);
		mr->SetMaterial(backmaterial);
	}

	// Logo
	{
		GameObject* logo = Object::Instantiate<GameObject>(eLayerType::Effect);
		Transform* tr = logo->GetComponent<Transform>();
		tr->SetSize(Vector3(1000.0f, 2400.0f, 1.0f));

		tr->SetPosition(Vector3(0.0f, 350.f, 0.0f));

		SpriteRenderer* sr = logo->AddComponent<SpriteRenderer>();

		std::weak_ptr<Mesh> mesh = ResourceManager::GetInstance()->Find<Mesh>(L"RectMesh");
		std::weak_ptr<Material> material = ResourceManager::GetInstance()->Find<Material>(L"SpriteMaterial");
		sr->SetMesh(mesh);
		sr->SetMaterial(material);

		Animator* animator = logo->AddComponent<Animator>();
		std::shared_ptr<Texture2D> tex = std::make_shared<Texture2D>();
		tex->Load(L"UI\\Logo.png");

		ResourceManager::GetInstance()->Insert<Texture2D>(L"LogoTex", tex);

		material.lock()->SetTexture(eTextureSlot::T0, tex);
		animator->Create(L"Logo", tex, Vector2(0.0f, 0.0f), 216.f, Vector2::Zero, 15, 0.1f);
		animator->Play(L"Logo");
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