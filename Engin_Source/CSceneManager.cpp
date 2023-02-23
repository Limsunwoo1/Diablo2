#include "CSceneManager.h"
#include "CRenderer.h"
#include "CResourceManager.h"
#include "CMeshRenderer.h"
#include "CShader.h"
#include "CMesh.h"
#include "CTexture.h"
#include "CMaterial.h"
#include "CPlayerScript.h"
#include "CCamera.h"
#include "CCameraScript.h"
#include "CSpriteRenderer.h"
#include "CGridScript.h"
#include "CFadeRenderer.h"
#include "CObject.h"

SceneManager::SceneManager()
	: mActiveScene(nullptr)
{
	
}
SceneManager::~SceneManager()
{
	
}

void SceneManager::Initalize()
{
	mActiveScene = new Scene();
	mActiveScene->Initalize();

	// GridObject;
	GameObject* gridObject = Object::Instantiate<GameObject>(eLayerType::Grid);
	MeshRenderer* gridMR = gridObject->AddComponent<MeshRenderer>();
	gridObject->AddComponent<GridScript>();

	gridMR->SetMesh(ResourceManager::GetInstance()->Find<Mesh>(L"RectMesh"));
	gridMR->SetMaterial(ResourceManager::GetInstance()->Find<Material>(L"GridMaterial"));

	//Camera GameObject
	GameObject* cameraObj = Object::Instantiate<GameObject>(eLayerType::Camera);
	Transform* cameraTr = cameraObj->GetComponent<Transform>();
	cameraTr->SetPosition(Vector3(0.0f, 0.0f, 0.0f));

	Camera* cameraComp = cameraObj->AddComponent<Camera>();
	cameraComp->TrunLayerMast(eLayerType::UI, false);
	cameraComp->TrunLayerMast(eLayerType::Fade, false);
	cameraComp->RegisterCameraInRenderer();

	CameraScript* cameraScript = cameraObj->AddComponent<CameraScript>();

	// UICamera
	GameObject* UIcameraObj = Object::Instantiate<GameObject>(eLayerType::UI);
	Transform* UIcameraTr = UIcameraObj->GetComponent<Transform>();
	UIcameraTr->SetPosition(Vector3(0.0f, 0.0f, 0.0f));

	Camera* UIcameraComp = UIcameraObj->AddComponent<Camera>();
	UIcameraComp->SetProjectionType(Camera::eProjectionType::Orthographic);
	UIcameraComp->DisableLayerMasks();
	UIcameraComp->TrunLayerMast(eLayerType::UI, true);
	UIcameraComp->TrunLayerMast(eLayerType::Fade, true);

	// SMILE RECT
	GameObject* obj = Object::Instantiate<GameObject>(eLayerType::Player);
	Transform* tr = obj->GetComponent<Transform>();
	tr->SetPosition(Vector3(0.0f, 0.0f, 10.0f));
	tr->SetScale(Vector3(1.0f, 1.0f, 1.0f));

	MeshRenderer* mr = obj->AddComponent<MeshRenderer>();

	shared_ptr<Mesh> mesh = ResourceManager::GetInstance()->Find<Mesh>(L"RectMesh");
	shared_ptr<Material> material = ResourceManager::GetInstance()->Find<Material>(L"RectMaterial");

	Vector2 vec2(0.1f, 0.1f);
	material->SetData(eGpuParam::Vector2, &vec2);

	mr->SetMaterial(material);
	mr->SetMesh(mesh);

	PlayerScript* SmileScript = obj->AddComponent<PlayerScript>();

	// SMILE RECT CHILD
	GameObject* SmileChild = Object::Instantiate<GameObject>(eLayerType::Player, tr);

	// Transmform
	Transform* SmilechildTr = SmileChild->GetComponent<Transform>();
	SmilechildTr->SetPosition(Vector3(2.0f, 0.0f, 0.0f));
	SmilechildTr->SetScale(Vector3(1.0f, 1.0f, 1.0f));

	// Renderer
	MeshRenderer* childMr = SmileChild->AddComponent<MeshRenderer>();

	// Material
	std::shared_ptr<Material> chileMaterial = ResourceManager::GetInstance()->Find<Material>(L"RectMaterial");

	childMr->SetMaterial(chileMaterial);
	childMr->SetMesh(mesh);


	// Player
	GameObject* Player = Object::Instantiate<GameObject>(eLayerType::Player);
	Transform* playerTR = Player->GetComponent<Transform>();
	playerTR->SetPosition(Vector3(0.0f, 0.0f, 10.0f));
	playerTR->SetScale(Vector3(3.0f, 3.0f, 3.0f));

	SpriteRenderer* playerSPR = Player->AddComponent<SpriteRenderer>();

	std::shared_ptr<Mesh> playerMesh = ResourceManager::GetInstance()->Find<Mesh>(L"PlayerMesh");
	std::shared_ptr<Material> playermaterial = ResourceManager::GetInstance()->Find<Material>(L"DiabloMaterial");

	playerSPR->SetMesh(playerMesh);
	playerSPR->SetMaterial(playermaterial);

	// LightObj
	GameObject* spriteObj = Object::Instantiate<GameObject>(eLayerType::Player);
	Transform* spriteTr = spriteObj->GetComponent<Transform>();
	spriteTr->SetPosition(Vector3(0.0f, 0.0f, 10.0f));
	spriteTr->SetScale(Vector3(6.0f, 6.0f, 3.0f));

	SpriteRenderer* sr = spriteObj->AddComponent<SpriteRenderer>();

	std::shared_ptr<Material> spriteMaterial = ResourceManager::GetInstance()->Find<Material>(L"SpriteMaterial");

	/*Vector2 vec2(1.0f, 1.0f);
	spriteMaterial->SetData(eGPUParam::Vector2, &vec2);*/

	sr->SetMaterial(spriteMaterial);
	sr->SetMesh(mesh);

	// HpBar
	GameObject* HPbarObj = Object::Instantiate<GameObject>(eLayerType::UI);
	Transform* HPbarTr = HPbarObj->GetComponent<Transform>();
	HPbarTr->SetPosition(Vector3(-5.0f, 3.0f, 10.0f));
	HPbarTr->SetScale(Vector3(1.0f, 1.0f, 1.0f));

	SpriteRenderer* HPbarsr = HPbarObj->AddComponent<SpriteRenderer>();

	std::shared_ptr<Mesh> hpmesh = ResourceManager::GetInstance()->Find<Mesh>(L"RectMesh");
	std::shared_ptr<Material> HPbarMaterial = ResourceManager::GetInstance()->Find<Material>(L"UIMaterial");

	HPbarsr->SetMaterial(HPbarMaterial);
	HPbarsr->SetMesh(hpmesh);

	// Fade
	GameObject* Fade = Object::Instantiate<GameObject>(eLayerType::Fade);

	FadeRenderer* fadeRenderer = Fade->AddComponent<FadeRenderer>();

	std::shared_ptr<Mesh> fademesh = ResourceManager::GetInstance()->Find<Mesh>(L"FadeMesh");
	std::shared_ptr<Material>fadeMaterial = ResourceManager::GetInstance()->Find<Material>(L"FadeMaterial");

	fadeRenderer->SetMesh(fademesh);
	fadeRenderer->SetMaterial(fadeMaterial);

	mActiveScene->Initalize();
}

void SceneManager::Update()
{
	mActiveScene->Update();
}

void SceneManager::FixedUpdate()
{
	mActiveScene->FixedUpdate();
}

void SceneManager::Render()
{
	mActiveScene->Render();
}

void SceneManager::Rlease()
{
	delete mActiveScene;
	mActiveScene = nullptr;
}
