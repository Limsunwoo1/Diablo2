#include "CLoaddingScene.h"
#include "CCamera.h"
#include "CAnimator.h"
#include "CTransform.h"
#include "CSceneManager.h"
#include "CObject.h"
#include "CLight.h"
#include "CSpriteRenderer.h"
#include "CResourceManager.h"
#include "CCameraScript.h"
#include "CCamera.h"
#include "CBackGround.h"
#include "CMeshRenderer.h"
#include "CTime.h"
#include "CObjectManager.h"


LoaddingScene::LoaddingScene()
	: Scene(eSceneType::Loadding)
	, mTime(2.5f)
	, NextScene(eSceneType::MainTitle)
{
}

LoaddingScene::~LoaddingScene()
{
}

void LoaddingScene::Initalize()
{
	// Light
	{
		GameObject* directionalLight = Object::Instantiate<GameObject>(eLayerType::None, this);
		Transform* tr = directionalLight->GetComponent<Transform>();
		tr->SetPosition(Vector3(0.0f, 0.0f, -100.f));
		Light* lightcomp = directionalLight->AddComponent<Light>();
		lightcomp->SetType(eLightType::Directional);
		lightcomp->SetDiffuse(Vector4(1.f, 1.0f, 1.f, 1.0f));
	}

	{
		// Main Camera Game Object
		GameObject* cameraObj = Object::Instantiate<GameObject>(eLayerType::Camera,this);
		Camera* cameraComp = cameraObj->AddComponent<Camera>();
		//cameraComp->RegisterCameraInRenderer();
		//cameraComp->TurnLayerMask(eLayerType::UI, false);
		cameraObj->AddComponent<CameraScript>();
		cameraComp->SetProjectionType(Camera::eProjectionType::Orthographic);
		mMainCamera = cameraComp;

		cameraObj->GetComponent<Transform>()->SetPosition(Vector3(0.0f, 0.0f, -100.f));
	}

	//renderer::cameras[0] = cameraComp;
	// BackGround
	{
		BackGround* ground = Object::Instantiate<BackGround>(eLayerType::BackGround,this);
		ground->SetName(L"BackGround");

		Transform* tr = ground->GetComponent<Transform>();
		tr->SetSize(Vector3(1600.f, 900.f, 1.0f));

		tr->SetPosition(mMainCamera->GetOwner()->GetComponent<Transform>()->GetPosition());
		tr->SetPosition(Vector3(0.0f, 0.0f, 1.0f));

		std::weak_ptr<Texture2D> tex = ResourceManager::GetInstance()->Load<Texture2D>(L"LoadSceneBack", L"BlackBackGround.png");
		ground->SetTexture(tex);

		MeshRenderer* mr = ground->AddComponent<MeshRenderer>();

		std::weak_ptr<Mesh> mesh = ResourceManager::GetInstance()->Find<Mesh>(L"RectMesh");
		std::weak_ptr<Material> backmaterial = ResourceManager::GetInstance()->Find<Material>(L"RectMaterial");

		mr->SetMesh(mesh);
		mr->SetMaterial(backmaterial);
	}

	// Logo
	{
		GameObject* logo = Object::Instantiate<GameObject>(eLayerType::Effect,this);
		Transform* tr = logo->GetComponent<Transform>();
		tr->SetSize(Vector3(5500.f, 500, 1.0f));

		SpriteRenderer* sr = logo->AddComponent<SpriteRenderer>();

		std::weak_ptr<Mesh> mesh = ResourceManager::GetInstance()->Find<Mesh>(L"RectMesh");
		std::weak_ptr<Material> material = ResourceManager::GetInstance()->Find<Material>(L"SpriteMaterial");
		sr->SetMesh(mesh);
		sr->SetMaterial(material);

		Animator* animator = logo->AddComponent<Animator>();
		std::shared_ptr<Texture2D> tex = std::make_shared<Texture2D>();
		tex->Load(L"UI\\LodingSprite//LodingSprite1.png");

		ResourceManager::GetInstance()->Insert<Texture2D>(L"LoaddingTex", tex);

		material.lock()->SetTexture(eTextureSlot::T0, tex);
		animator->Create(L"Loadding", tex, Vector2::Zero, Vector2(796.f,537.f),
			Vector2::Zero, Vector2(796.f * 10.f, 537.f),10, 0.1f);
		animator->Play(L"Loadding");

		LoadObject = logo;
	}
}

void LoaddingScene::Update()
{
	mTime -= Time::GetInstance()->DeltaTime();

	if (mTime <= 0)
	{
		mTime = 1.7f;
		SceneManager::GetInstance()->LoadScene(NextScene);
	}
	Scene::Update();
}

void LoaddingScene::FixedUpdate()
{
	Scene::FixedUpdate();
}

void LoaddingScene::Render()
{
	Scene::Render();
}

void LoaddingScene::OnEnter()
{
	//Renderer::mainCamera = mMainCamera;
	LoadObject->GetComponent<Animator>()->Play(L"Loadding", false);
	ObjectManager::GetInstance()->Clear();
	mTime = 1.7f;
}
