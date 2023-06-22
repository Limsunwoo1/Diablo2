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
#include "CButton.h"
#include "CUIManager.h"
#include "CSelectSystemButton.h"

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
		GameObject* directionalLight = Object::Instantiate<GameObject>(eLayerType::None,this);
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
		cameraComp->TurnLayerMask(eLayerType::UI, false);
		cameraObj->AddComponent<CameraScript>();
		cameraComp->SetProjectionType(Camera::eProjectionType::Orthographic);
		Renderer::mainCamera = cameraComp;
		mMainCamera = cameraComp;
	}

	{
		// Main Camera Game Object
		GameObject* uicameraObj = Object::Instantiate<GameObject>(eLayerType::Camera, this);
		uicameraObj->SetName(L"MainTitleUiCam");
		Camera* uicameraComp = uicameraObj->AddComponent<Camera>();
		uicameraComp->DisableLayerMasks();
		//cameraComp->RegisterCameraInRenderer();
		uicameraComp->TurnLayerMask(eLayerType::UI, true);
		uicameraComp->SetProjectionType(Camera::eProjectionType::Orthographic);
		Renderer::UiCamera = uicameraComp;
		mUiCamera = uicameraComp;
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
		GameObject* logo = Object::Instantiate<GameObject>(eLayerType::Effect, this);
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

	{
		SelectSystemButton* button = new SelectSystemButton();
		button->Initalize();

		SpriteRenderer* sr = button->AddComponent<SpriteRenderer>();
		
		std::weak_ptr<Mesh> mesh = ResourceManager::GetInstance()->Find<Mesh>(L"RectMesh");
		std::weak_ptr<Material> mater = ResourceManager::GetInstance()->Find<Material>(L"Button1Material");

		sr->SetMesh(mesh);
		sr->SetMaterial(mater);

		std::weak_ptr<Texture2D> tex1 = ResourceManager::GetInstance()->Load<Texture2D>(L"playStart", L"..//Resource//UI//PlayStart.png");
		std::weak_ptr<Texture2D> tex2 = ResourceManager::GetInstance()->Load<Texture2D>(L"playStartClick", L"..//Resource//UI//playstartCkilck.png");

		button->SetButtonTex(0, tex1);
		button->SetButtonTex(1, tex2);

		Object::Instantiate<SelectSystemButton>(eLayerType::UI, button, true, eSceneType::MainTitle);
		UIManager::GetInstance()->Push(L"StartButton", button);

		Transform* buttonTr = button->GetComponent<Transform>();
		buttonTr->SetPosition(Vector3(0.f, -100.f, 1.0f));
		buttonTr->SetSize(Vector3(550.f, 50.f, 1.0f));

		button->BindEvnet(std::bind(&MainTitleScene::NextScene, this));
	}

	{
		SelectSystemButton* button = new SelectSystemButton();
		button->Initalize();

		SpriteRenderer* sr = button->AddComponent<SpriteRenderer>();

		std::weak_ptr<Mesh> mesh = ResourceManager::GetInstance()->Find<Mesh>(L"RectMesh");
		std::weak_ptr<Material> mater = ResourceManager::GetInstance()->Find<Material>(L"Button2Material");

		sr->SetMesh(mesh);
		sr->SetMaterial(mater);

		std::weak_ptr<Texture2D> tex1 = ResourceManager::GetInstance()->Load<Texture2D>(L"PrveButton", L"..//Resource//UI//playEnd.png");
		std::weak_ptr<Texture2D> tex2 = ResourceManager::GetInstance()->Load<Texture2D>(L"PrveButtonClick", L"..//Resource//UI//playEndClick.png");

		button->SetButtonTex(0, tex1);
		button->SetButtonTex(1, tex2);

		Object::Instantiate<SelectSystemButton>(eLayerType::UI, button, true, eSceneType::MainTitle);
		UIManager::GetInstance()->Push(L"PrevButton", button);

		Transform* buttonTr = button->GetComponent<Transform>();
		buttonTr->SetPosition(Vector3(0.f, -350.f, 1.0f));
		buttonTr->SetSize(Vector3(550.f, 50.f, 1.0f));

		button->BindEvnet(std::bind(&MainTitleScene::PrevScene, this));
	}

	{
		SelectSystemButton* button = new SelectSystemButton();
		button->Initalize();

		SpriteRenderer* sr = button->AddComponent<SpriteRenderer>();

		std::weak_ptr<Mesh> mesh = ResourceManager::GetInstance()->Find<Mesh>(L"RectMesh");
		std::weak_ptr<Material> mater = ResourceManager::GetInstance()->Find<Material>(L"Button3Material");

		sr->SetMesh(mesh);
		sr->SetMaterial(mater);

		std::weak_ptr<Texture2D> tex1 = ResourceManager::GetInstance()->Load<Texture2D>(L"CreaptingStart", L"..//Resource//UI//CreaptingButton.png");
		std::weak_ptr<Texture2D> tex2 = ResourceManager::GetInstance()->Load<Texture2D>(L"CreaptingClick", L"..//Resource//UI//CreaptingButton.png");

		button->SetButtonTex(0, tex1);
		button->SetButtonTex(1, tex2);

		Object::Instantiate<SelectSystemButton>(eLayerType::UI, button, true, eSceneType::MainTitle);
		UIManager::GetInstance()->Push(L"Creapting", button);

		Transform* buttonTr = button->GetComponent<Transform>();
		buttonTr->SetPosition(Vector3(0.f, -180.f, 1.0f));
		buttonTr->SetSize(Vector3(550.f, 50.f, 1.0f));

		button->BindEvnet(std::bind(&MainTitleScene::ToolScene, this));
	}

	Scene::Initalize();
}

void MainTitleScene::Update()
{

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
	Renderer::mainCamera = GetMainCam();
	Renderer::UiCamera = GetUiCam();
}

void MainTitleScene::OnExit()
{
}

void MainTitleScene::NextScene()
{
	SceneManager::GetInstance()->LoadScene(eSceneType::Selecte);
}

void MainTitleScene::PrevScene()
{
	SceneManager::GetInstance()->LoadScene(eSceneType::Title);
}

void MainTitleScene::ToolScene()
{
	SceneManager::GetInstance()->LoadScene(eSceneType::Tool);
}
