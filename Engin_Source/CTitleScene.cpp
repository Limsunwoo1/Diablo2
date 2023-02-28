#pragma once
#include "CTitleScene.h"
#include "CRenderer.h"
#include "CMeshRenderer.h"
#include "CSpriteRenderer.h"
#include "CResourceManager.h"
#include "CTexture.h"
#include "CPlayerScript.h"
#include "CCameraScript.h"
#include "CGridScript.h"
#include "CObject.h"


TitleScene::TitleScene()
{

}
TitleScene::~TitleScene()
{

}

void TitleScene::Initalize()
{
	Scene::Initalize();

	// Main Camera Game Object
	GameObject* cameraObj = Object::Instantiate<GameObject>(eLayerType::Camera);
	Camera* cameraComp = cameraObj->AddComponent<Camera>();
	cameraComp->RegisterCameraInRenderer();
	cameraComp->TurnLayerMask(eLayerType::UI, false);
	cameraObj->AddComponent<CameraScript>();
	//renderer::cameras[0] = cameraComp;

	// UI Camera
	GameObject* cameraUIObj = Object::Instantiate<GameObject>(eLayerType::Camera);
	Camera* cameraUIComp = cameraUIObj->AddComponent<Camera>();
	cameraUIComp->SetProjectionType(Camera::eProjectionType::Orthographic);
	cameraUIComp->DisableLayerMasks();
	cameraUIComp->TurnLayerMask(eLayerType::UI, true);

	// Grid Object
	GameObject* gridObject = Object::Instantiate<GameObject>(eLayerType::None);
	MeshRenderer* gridMr = gridObject->AddComponent<MeshRenderer>();
	gridMr->SetMesh(ResourceManager::GetInstance()->Find<Mesh>(L"RectMesh"));
	gridMr->SetMaterial(ResourceManager::GetInstance()->Find<Material>(L"GridMaterial"));
	GridScript* gridScript = gridObject->AddComponent<GridScript>();
	gridScript->SetCamera(cameraComp);




	// Light Object
	GameObject* spriteObj = Object::Instantiate<GameObject>(eLayerType::Player);
	spriteObj->SetName(L"LIGHT");
	Transform* spriteTr = spriteObj->GetComponent<Transform>();
	spriteTr->SetPosition(Vector3(0.0f, 0.0f, 11.0f));
	spriteTr->SetScale(Vector3(5.0f, 5.0f, 1.0f));

	SpriteRenderer* sr = spriteObj->AddComponent<SpriteRenderer>();
	std::shared_ptr<Mesh> mesh = ResourceManager::GetInstance()->Find<Mesh>(L"RectMesh");
	std::shared_ptr<Material> spriteMaterial = ResourceManager::GetInstance()->Find<Material>(L"SpriteMaterial");
	sr->SetMaterial(spriteMaterial);
	sr->SetMesh(mesh);


	//SMILE RECT
	GameObject* obj = Object::Instantiate<GameObject>(eLayerType::Player);
	obj->SetName(L"SMILE");
	Transform* tr = obj->GetComponent<Transform>();
	tr->SetPosition(Vector3(-3.0f, 0.0f, 11.0f));
	tr->SetRotation(Vector3(0.0f, 0.0f, XM_PIDIV2));
	tr->SetScale(Vector3(1.0f, 1.0f, 1.0f));

	MeshRenderer* mr = obj->AddComponent<MeshRenderer>();
	std::shared_ptr<Material> mateiral = ResourceManager::GetInstance()->Find<Material>(L"RectMaterial");
	mr->SetMaterial(mateiral);
	mr->SetMesh(mesh);
	obj->AddComponent<PlayerScript>();

	//SMILE RECT CHild
	GameObject* child = Object::Instantiate<GameObject>(eLayerType::Player);
	child->SetName(L"SMILE");
	Transform* childTr = child->GetComponent<Transform>();
	childTr->SetPosition(Vector3(2.0f, 0.0f, 0.0f));
	childTr->SetScale(Vector3(1.0f, 1.0f, 1.0f));
	childTr->SetParent(tr);

	MeshRenderer* childMr = child->AddComponent<MeshRenderer>();
	std::shared_ptr<Material> childmateiral = ResourceManager::GetInstance()->Find<Material>(L"RectMaterial");
	childMr->SetMaterial(childmateiral);
	childMr->SetMesh(mesh);

	// HPBAR
	GameObject* hpBar = Object::Instantiate<GameObject>(eLayerType::Player);
	hpBar->SetName(L"HPBAR");
	Transform* hpBarTR = hpBar->GetComponent<Transform>();
	hpBarTR->SetPosition(Vector3(-5.0f, 3.0f, 12.0f));
	hpBarTR->SetScale(Vector3(1.0f, 1.0f, 1.0f));

	SpriteRenderer* hpsr = hpBar->AddComponent<SpriteRenderer>();
	hpBar->AddComponent(hpsr);
	std::shared_ptr<Mesh> hpmesh = ResourceManager::GetInstance()->Find<Mesh>(L"RectMesh");
	std::shared_ptr<Material> hpspriteMaterial = ResourceManager::GetInstance()->Find<Material>(L"UIMaterial");
	hpsr->SetMesh(hpmesh);
	hpsr->SetMaterial(hpspriteMaterial);
}

void TitleScene::Update()
{
	Scene::Update();
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