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
	// Main Camera Game Object
	GameObject* cameraObj = Object::Instantiate<GameObject>(eLayerType::Camera);
	Camera* cameraComp = cameraObj->AddComponent<Camera>();
	//cameraComp->RegisterCameraInRenderer();
	cameraComp->TurnLayerMask(eLayerType::UI, false);
	cameraObj->AddComponent<CameraScript>();
	cameraComp->SetProjectionType(Camera::eProjectionType::Orthographic);
	Renderer::mainCamera = cameraComp;

	RECT winRect;
	GetClientRect(Application.GetHwnd(), &winRect);

	float width = (float)(winRect.right - winRect.left);
	float height = (float)(winRect.bottom - winRect.top);

	//renderer::cameras[0] = cameraComp;
	// BackGround
	{
		/*BackGround* ground = Object::Instantiate<BackGround>(eLayerType::BackGround);
		ground->SetName(L"BackGround");
		
		ground->FindTextuer2D(L"TitleTexture", L"UI\\Title_01.png");
		Transform* tr = ground->GetComponent<Transform>();
		tr->SetScale(Vector3(8.0f, 7.0f, 0.0f));

		MeshRenderer* mr = ground->AddComponent<MeshRenderer>();

		std::shared_ptr<Mesh> mesh = ResourceManager::GetInstance()->Find<Mesh>(L"FadeMesh");
		std::shared_ptr<Material> material = ResourceManager::GetInstance()->Find<Material>(L"RectMaterial");
		mr->SetMesh(mesh);
		mr->SetMaterial(material);*/
	}

	// BackGround2
	{
		BackGround* ground = Object::Instantiate<BackGround>(eLayerType::BackGround);
		ground->SetName(L"BackGround2");

		ground->FindTextuer2D(L"TitleTexture2", L"UI\\Title_02.png");
		Transform* tr = ground->GetComponent<Transform>();

		tr->SetScale(Vector3(width / 100.f, height / 100.f, 0.0f));

		MeshRenderer* mr = ground->AddComponent<MeshRenderer>();

		std::shared_ptr<Mesh> mesh = ResourceManager::GetInstance()->Find<Mesh>(L"FadeMesh");
		std::shared_ptr<Material> material = ResourceManager::GetInstance()->Find<Material>(L"RectMaterial");
		mr->SetMesh(mesh);
		mr->SetMaterial(material);

		//ground->Paused();
	}

	// Logo
	{
		GameObject* logo = Object::Instantiate<GameObject>(eLayerType::Player);
		Transform* tr = logo->GetComponent<Transform>();
		tr->SetScale(Vector3(8.0f, 12.0f, 1.0f));

		SpriteRenderer* sr = logo->AddComponent<SpriteRenderer>();
		
		std::shared_ptr<Mesh> mesh = ResourceManager::GetInstance()->Find<Mesh>(L"RectMesh");
		std::shared_ptr<Material> material = ResourceManager::GetInstance()->Find<Material>(L"SpriteMaterial");
		sr->SetMesh(mesh);
		sr->SetMaterial(material);

		Animator* animator = logo->AddComponent<Animator>();
		std::shared_ptr<Texture2D> tex = std::make_shared<Texture2D>();
		tex->Load(L"UI\\Logo.png");

		animator->Create(L"Logo", tex, Vector2(0.0f, 0.0f), 216.f, Vector2::Zero, 15, 0.1f);
		animator->Play(L"Logo");
		//ground->Paused();
	}

	//SMILE RECT
	//{
	//	GameObject* obj = Object::Instantiate<GameObject>(eLayerType::Player);
	//	obj->SetName(L"SMILE");
	//	Transform* tr = obj->GetComponent<Transform>();
	//	tr->SetPosition(Vector3(0.0f, 0.0f, 5.0f));
	//	/*tr->SetRotation(Vector3(0.0f, 0.0f, XM_PIDIV2));
	//	tr->SetScale(Vector3(1.0f, 1.0f, 1.0f));*/
	//	Collider2D* collider = obj->AddComponent<Collider2D>();
	//	collider->SetType(eColliderType::Circle);
	//	collider->SetSize(Vector2(1.0f, 1.0f));

	//	MeshRenderer* mr = obj->AddComponent<MeshRenderer>();
	//	std::shared_ptr<Material> mateiral = ResourceManager::GetInstance()->Find<Material>(L"RectMaterial");
	//	mr->SetMaterial(mateiral);
	//	std::shared_ptr<Mesh> mesh = ResourceManager::GetInstance()->Find<Mesh>(L"RectMesh");
	//	mr->SetMesh(mesh);
	//	obj->AddComponent<PlayerScript>();
	//	Object::DontDestroyOnLoad(obj);
	//}

	//SMILE RECT 2
	//{
	//	GameObject* obj = Object::Instantiate<GameObject>(eLayerType::Monster);
	//	obj->SetName(L"SMILE");
	//	Transform* tr = obj->GetComponent<Transform>();
	//	tr->SetPosition(Vector3(2.0f, 0.0f, 5.0f));
	//	/*tr->SetRotation(Vector3(0.0f, 0.0f, XM_PIDIV2));
	//	tr->SetScale(Vector3(1.0f, 1.0f, 1.0f));*/
	//	Collider2D* collider = obj->AddComponent<Collider2D>();
	//	collider->SetType(eColliderType::Circle);
	//	collider->SetSize(Vector2(1.0f, 1.0f));

	//	MeshRenderer* mr = obj->AddComponent<MeshRenderer>();
	//	std::shared_ptr<Material> mateiral = ResourceManager::GetInstance()->Find<Material>(L"RectMaterial");
	//	mr->SetMaterial(mateiral);
	//	std::shared_ptr<Mesh> mesh = ResourceManager::GetInstance()->Find<Mesh>(L"RectMesh");
	//	mr->SetMesh(mesh);
	//	Object::DontDestroyOnLoad(obj);
	//}

	//SMILE RECT CHild
	/*GameObject* child = Object::Instantiate<GameObject>(eLayerType::Player);
	child->SetName(L"SMILE");
	Transform* childTr = child->GetComponent<Transform>();
	childTr->SetPosition(Vector3(2.0f, 0.0f, 0.0f));
	childTr->SetScale(Vector3(1.0f, 1.0f, 1.0f));
	childTr->SetParent(tr);

	MeshRenderer* childMr = child->AddComponent<MeshRenderer>();
	std::shared_ptr<Material> childmateiral = ResourceManager::GetInstance()->Find<Material>(L"RectMaterial");
	childMr->SetMaterial(childmateiral);
	childMr->SetMesh(mesh);*/

	// HPBAR
	/*GameObject* hpBar = Object::Instantiate<GameObject>(eLayerType::Player);
	hpBar->SetName(L"HPBAR");
	Transform* hpBarTR = hpBar->GetComponent<Transform>();
	hpBarTR->SetPosition(Vector3(-5.0f, 3.0f, 12.0f));
	hpBarTR->SetScale(Vector3(1.0f, 1.0f, 1.0f));*/

	/*SpriteRenderer* hpsr = hpBar->AddComponent<SpriteRenderer>();
	hpBar->AddComponent(hpsr);
	std::shared_ptr<Mesh> hpmesh = ResourceManager::GetInstance()->Find<Mesh>(L"RectMesh");
	std::shared_ptr<Material> hpspriteMaterial = ResourceManager::GetInstance()->Find<Material>(L"UIMaterial");
	hpsr->SetMesh(hpmesh);
	hpsr->SetMaterial(hpspriteMaterial);*/

	CollisionManager::GetInstance()->CollisionlayerCheck(eLayerType::Player, eLayerType::Monster);
	Scene::Initalize();
}

void TitleScene::Update()
{
	Scene::Update();

	if (Input::GetInstance()->GetKeyDown(eKeyCode::LBTN))
	{
		SceneManager::GetInstance()->LoadScene(eSceneType::Selecte);
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