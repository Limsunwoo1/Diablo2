#include "CEnddingScene.h"
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
#include "CLoaddingScene.h"
#include "CLavaTile.h"
#include "CAudioListner.h"
#include "CAudioSource.h"
#include "CAudioClip.h"

EnddingScene::EnddingScene()
	: Scene(eSceneType::Endding)
{
}

EnddingScene::~EnddingScene()
{
}

void EnddingScene::Initalize()
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
		GameObject* cameraObj = Object::Instantiate<GameObject>(eLayerType::Camera, this);
		Camera* cameraComp = cameraObj->AddComponent<Camera>();
		cameraObj->GetComponent<Transform>()->SetPosition(Vector3(0, 0, 1.0f));
		//cameraComp->RegisterCameraInRenderer();
		cameraComp->TurnLayerMask(eLayerType::UI, false);
		//cameraComp->TurnLayerMask(eLayerType::Tile, false);
		cameraObj->AddComponent<CameraScript>();
		cameraComp->SetProjectionType(Camera::eProjectionType::Orthographic);
		mMainCamera = cameraComp;

		cameraObj->AddComponent<AudioListener>();
		AudioSource* source = cameraObj->AddComponent<AudioSource>();

		std::weak_ptr<AudioClip> clip = ResourceManager::GetInstance()->
			Load<AudioClip>(L"EndSound", L"SoundResource\\End.mp3");

		source->SetClip(clip);
		source->SetLoop(true);
	}
	// Ui Camera
	{
		GameObject* uiCamera = Object::Instantiate<GameObject>(eLayerType::Camera, this);
		Camera* uiCameraComp = uiCamera->AddComponent<Camera>();
		uiCameraComp->DisableLayerMasks();
		uiCameraComp->TurnLayerMask(eLayerType::UI, true);

		uiCameraComp->SetProjectionType(Camera::eProjectionType::Orthographic);
		mUiCamera = uiCameraComp;
	}

	RECT winRect;
	GetClientRect(Application.GetHwnd(), &winRect);

	float width = (float)(winRect.right - winRect.left);
	float height = (float)(winRect.bottom - winRect.top);

	//renderer::cameras[0] = cameraComp;
	// BackGround
	{
		GameObject* ground = Object::Instantiate<GameObject>(eLayerType::BackGround,this);


		Transform* tr = ground->GetComponent<Transform>();
		tr->SetSize(Vector3(16000.f, 900.f, 1.0f));
		tr->SetPosition(mMainCamera->GetOwner()->GetComponent<Transform>()->GetPosition());


		std::weak_ptr<Texture2D> tex = ResourceManager::GetInstance()->Load<Texture2D>(L"End", L"EndScene.png");
		Animator* animator = ground->AddComponent<Animator>();

		animator->Create(L"EndScene", tex, Vector2::Zero, Vector2(394.f, 293.f), Vector2::Zero,Vector2(394.f * 9.f, 293.f), 19, 0.1f);
		animator->Play(L"EndScene", true);


		SpriteRenderer* mr = ground->AddComponent<SpriteRenderer>();

		std::weak_ptr<Mesh> mesh = ResourceManager::GetInstance()->Find<Mesh>(L"RectMesh");
		std::weak_ptr<Material> backmaterial = ResourceManager::GetInstance()->Find<Material>(L"SpriteMaterial");

		mr->SetMesh(mesh);
		mr->SetMaterial(backmaterial);
	}
}

void EnddingScene::Update()
{
	Scene::Update();
}

void EnddingScene::FixedUpdate()
{
	Scene::FixedUpdate();
}

void EnddingScene::Render()
{
	Scene::Render();
}

void EnddingScene::Destroy()
{
}

void EnddingScene::OnEnter()
{
	GetMainCam()->GetOwner()->GetComponent<AudioSource>()->Play(0.15f);
	Renderer::mainCamera = GetMainCam();
	Renderer::UiCamera = GetUiCam();
}

void EnddingScene::OnExit()
{
}