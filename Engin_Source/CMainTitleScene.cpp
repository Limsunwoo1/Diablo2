#include "CMainTitleScene.h"
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

MainTitleScene::MainTitleScene()
	: Scene(eSceneType::MainTitle)
{
}

MainTitleScene::~MainTitleScene()
{
}

void MainTitleScene::Initalize()
{
	// Light
	{
		GameObject* directionalLight = Object::Instantiate<GameObject>(eLayerType::Player,this);
		Transform* tr = directionalLight->GetComponent<Transform>();
		tr->SetPosition(Vector3(0.0f, 0.0f, -100.f));
		Light* lightcomp = directionalLight->AddComponent<Light>();
		lightcomp->SetType(eLightType::Directional);
		lightcomp->SetDiffuse(Vector4(1.f, 1.0f, 1.f, 1.0f));
	}

	{
		// Main Camera Game Object
		GameObject* cameraObj = Object::Instantiate<GameObject>(eLayerType::Camera, this);
		Camera* cameraComp = cameraObj->AddComponent<Camera>();
		//cameraComp->RegisterCameraInRenderer();
		//cameraComp->TurnLayerMask(eLayerType::UI, false);
		cameraObj->AddComponent<CameraScript>();
		cameraComp->SetProjectionType(Camera::eProjectionType::Orthographic);
		Renderer::mainCamera = cameraComp;
	}

	//BackGround2
	{
		BackGround* ground = Object::Instantiate<BackGround>(eLayerType::BackGround, this);
		ground->SetName(L"BackGround2");

		Transform* tr = ground->GetComponent<Transform>();
		tr->SetSize(Vector3(1600.f, 900.f, 1.0f));

		std::weak_ptr<Texture2D> tex = ResourceManager::GetInstance()->Load<Texture2D>(L"Title2", L"UI//Title_02.png");
		ground->SetTexture(tex);

		MeshRenderer* mr = ground->AddComponent<MeshRenderer>();
		std::weak_ptr<Mesh> mesh = ResourceManager::GetInstance()->Find<Mesh>(L"RectMesh");
		std::weak_ptr<Material> material = ResourceManager::GetInstance()->Find<Material>(L"RectMaterial");

		mr->SetMesh(mesh);
		mr->SetMaterial(material);
	}

	// Logo
	{
		GameObject* logo = Object::Instantiate<GameObject>(eLayerType::Player, this);
		Transform* tr = logo->GetComponent<Transform>();
		tr->SetSize(Vector3(1000.0f, 3500.f, 1.0f));
		tr->SetPosition(Vector3(0.0f, 350.f, 0.0f));

		SpriteRenderer* sr = logo->AddComponent<SpriteRenderer>();

		std::weak_ptr<Mesh> mesh = ResourceManager::GetInstance()->Find<Mesh>(L"RectMesh");
		std::weak_ptr<Material> material = ResourceManager::GetInstance()->Find<Material>(L"SpriteMaterial");
		sr->SetMesh(mesh);
		sr->SetMaterial(material);

		Animator* animator = logo->AddComponent<Animator>();
		std::weak_ptr<Texture2D> tex = ResourceManager::GetInstance()->Find<Texture2D>(L"LogoTex");

		material.lock()->SetTexture(eTextureSlot::T0, tex);
		animator->Create(L"Logo", tex, Vector2(0.0f, 0.0f), 216.f, Vector2::Zero, 15, 0.1f);
		animator->Play(L"Logo");
	}

	Scene::Initalize();
}

void MainTitleScene::Update()
{
	if (Input::GetInstance()->GetKeyDown(eKeyCode::LBTN))
	{
		SceneManager::GetInstance()->LoadScene(eSceneType::Selecte);
	}

	Scene::Update();
}

void MainTitleScene::FixedUpdate()
{
	Scene::FixedUpdate();
}

void MainTitleScene::Render()
{
	Scene::Render();
}

void MainTitleScene::OnEnter()
{
}

void MainTitleScene::OnExit()
{
}
