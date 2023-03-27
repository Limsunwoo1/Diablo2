#include "CPlayScene.h"
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
#include "CAnimator.h"
#include "CPlayer.h"
#include "CLight.h"
#include "CpaintShader.h"



PlayScene::PlayScene()
	: Scene(eSceneType::Play)
{

}
PlayScene::~PlayScene()
{

}

void PlayScene::Initalize()
{
	// PaintShader
	
		shared_ptr<PaintShader> paintShader = ResourceManager::GetInstance()->Find<PaintShader>(L"PaintShader");
		shared_ptr<Texture2D> paintTex = ResourceManager::GetInstance()->Find<Texture2D>(L"PaintTexture");

		paintShader->SetTarget(paintTex);
		paintShader->OnExcute();
	

	{
		GameObject* directionalLight = Object::Instantiate<GameObject>(eLayerType::Player, this);
		Transform* tr = directionalLight->GetComponent<Transform>();
		tr->SetPosition(Vector3(0.0f, 0.0f, -100.f));
		Light* lightcomp = directionalLight->AddComponent<Light>();
		lightcomp->SetType(eLightType::Directional);
		lightcomp->SetDiffuse(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
	}

	// Main Camera Game Object
	{
		GameObject* cameraObj = Object::Instantiate<GameObject>(eLayerType::Camera, this);
		Camera* cameraComp = cameraObj->AddComponent<Camera>();
		//cameraComp->RegisterCameraInRenderer();
		cameraComp->TurnLayerMask(eLayerType::UI, false);
		cameraObj->AddComponent<CameraScript>();
		cameraComp->SetProjectionType(Camera::eProjectionType::Orthographic);
		Renderer::mainCamera = cameraComp;
	}
	// Ui Camera
	{
		GameObject* uiCamera = Object::Instantiate<GameObject>(eLayerType::Camera, this);
		Camera* uiCameraComp = uiCamera->AddComponent<Camera>();
		uiCameraComp->DisableLayerMasks();
		uiCameraComp->TurnLayerMask(eLayerType::UI, true);

		uiCameraComp->SetProjectionType(Camera::eProjectionType::Orthographic);
		Renderer::Cameras->push_back(uiCameraComp);
	}

	// Player
	{
		Player* player = Object::Instantiate<Player>(eLayerType::Player, this);
		player->AddComponent<PlayerScript>();

		Collider2D* collider = player->AddComponent<Collider2D>();
		collider->SetSize(Vector2(1.0f, 1.0f));
		collider->SetType(eColliderType::Rect);

		Transform* tr = player->GetComponent<Transform>();
		tr->SetPosition(Vector3::Zero);
		tr->SetScale(Vector3(2.0f, 2.0f, 1.0f));

		SpriteRenderer* spr = player->AddComponent<SpriteRenderer>();
		std::shared_ptr<Mesh> mesh = ResourceManager::GetInstance()->Find<Mesh>(L"RectMesh");
		std::shared_ptr<Material> material = ResourceManager::GetInstance()->Find<Material>(L"SpriteMaterial");

		spr->SetMesh(mesh);
		spr->SetMaterial(material);

		player->InitAnimation();
		player->PlayAnimation(L"Walk0");

		//60 75.875f
	}

	//SMILE RECT
	{
		GameObject* obj = Object::Instantiate<GameObject>(eLayerType::BackGround, this);
		obj->SetName(L"SMILE");
		Transform* tr = obj->GetComponent<Transform>();
		tr->SetPosition(Vector3(2.0f, 0.0f, 5.0f));
		tr->SetScale(Vector3(2.0f, 2.0f, 2.0f));
		//tr->SetScale(Vector3(2.0f, 1.0f, 1.0f));
		//tr->SetRotation(Vector3(0.0f, 0.0f, XM_PIDIV2 / 2.0f));
		//tr->SetScale(Vector3(1.0f, 1.0f, 1.0f));
		Collider2D* collider = obj->AddComponent<Collider2D>();
		collider->SetSize(Vector2(2.0f, 2.0f));
		collider->SetType(eColliderType::Rect);
		//collider->SetCenter(Vector2(0.2f, 0.2f));
		//collider->SetSize(Vector2(1.5f, 1.5f));

		SpriteRenderer* sr = obj->AddComponent<SpriteRenderer>();
		std::shared_ptr<Material> mateiral = ResourceManager::GetInstance()->Find<Material>(L"noiseMaterial");
		mateiral->Clear();
		sr->SetMaterial(mateiral);
		std::shared_ptr<Mesh> mesh = ResourceManager::GetInstance()->Find<Mesh>(L"RectMesh");
		sr->SetMesh(mesh);
	}

	Scene::Initalize();
}

void PlayScene::Update()
{
	Scene::Update();

}

void PlayScene::FixedUpdate()
{
	shared_ptr<PaintShader> paintShader = ResourceManager::GetInstance()->Find<PaintShader>(L"PaintShader");
	paintShader->OnExcute();


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