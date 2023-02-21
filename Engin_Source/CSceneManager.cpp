#include "CSceneManager.h"
#include "CTransform.h"
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

	//Camera GameObject
	GameObject* cameraObj = new GameObject();
	Transform* cameraTr = new Transform();
	cameraTr->SetPosition(Vector3(0.0f, 0.0f, 0.0f));
	cameraObj->AddComponent(cameraTr);

	Camera* cameraComp = new Camera();
	cameraObj->AddComponent(cameraComp);
	cameraComp->TrunLayerMast(eLayerType::UI, false);

	CameraScript* cameraScript = new CameraScript();
	cameraObj->AddComponent(cameraScript);

	mActiveScene->AddGameObject(cameraObj, eLayerType::Camera);

	// UICamera
	GameObject* UIcameraObj = new GameObject();
	Transform* UIcameraTr = new Transform();
	UIcameraTr->SetPosition(Vector3(0.0f, 0.0f, 0.0f));
	UIcameraObj->AddComponent(UIcameraTr);

	Camera* UIcameraComp = new Camera();
	UIcameraComp->SetProjectionType(Camera::eProjectionType::Orthographic);
	UIcameraObj->AddComponent(UIcameraComp);

	UIcameraComp->DisableLayerMasks();
	UIcameraComp->TrunLayerMast(eLayerType::UI, true);

	mActiveScene->AddGameObject(UIcameraObj, eLayerType::Camera);

	// SMILE RECT
	GameObject* obj = new GameObject();
	Transform* tr = new Transform();
	tr->SetPosition(Vector3(0.0f, 0.0f, 10.0f));
	tr->SetScale(Vector3(1.0f, 1.0f, 1.0f));
	obj->AddComponent(tr);

	MeshRenderer* mr = new MeshRenderer();
	obj->AddComponent(mr);

	shared_ptr<Mesh> mesh = ResourceManager::GetInstance()->Find<Mesh>(L"RectMesh");
	shared_ptr<Material> material = ResourceManager::GetInstance()->Find<Material>(L"RectMaterial");

	Vector2 vec2(0.1f, 0.1f);
	material->SetData(eGpuParam::Vector2, &vec2);

	mr->SetMaterial(material);
	mr->SetMesh(mesh);

	PlayerScript* SmileScript = new PlayerScript();
	obj->AddComponent(SmileScript);

	mActiveScene->AddGameObject(obj, eLayerType::Player);

	// SMILE RECT CHILD
	GameObject* SmileChild = new GameObject();

	// Transmform
	Transform* SmileSchildTr = new Transform();
	SmileSchildTr->SetPosition(Vector3(2.0f, 0.0f, 0.0f));
	SmileSchildTr->SetScale(Vector3(1.0f, 1.0f, 1.0f));
	SmileSchildTr->SetParent(tr);

	SmileChild->AddComponent(SmileSchildTr);

	// Renderer
	MeshRenderer* childMr = new MeshRenderer();
	SmileChild->AddComponent(childMr);

	// Material
	std::shared_ptr<Material> chileMaterial = ResourceManager::GetInstance()->Find<Material>(L"RectMaterial");

	childMr->SetMaterial(chileMaterial);
	childMr->SetMesh(mesh);

	mActiveScene->AddGameObject(SmileChild, eLayerType::Player);

	// Player
	GameObject* Player = new GameObject();
	Transform* playerTR = new Transform();
	playerTR->SetPosition(Vector3(0.0f, 0.0f, 10.0f));
	playerTR->SetScale(Vector3(3.0f, 3.0f, 3.0f));
	Player->AddComponent(playerTR);

	SpriteRenderer* playerSPR = new SpriteRenderer();
	Player->AddComponent(playerSPR);

	std::shared_ptr<Mesh> playerMesh = ResourceManager::GetInstance()->Find<Mesh>(L"PlayerMesh");
	std::shared_ptr<Material> playermaterial = ResourceManager::GetInstance()->Find<Material>(L"DiabloMaterial");

	playerSPR->SetMesh(playerMesh);
	playerSPR->SetMaterial(playermaterial);

	//Vector2 playervec2(1.0f, 1.0f);
	//playermaterial->SetData(graphics::eGpuParam::Vector2, &playervec2);

	mActiveScene->AddGameObject(Player, eLayerType::Player);


	// LightObj
	GameObject* spriteObj = new GameObject();
	Transform* spriteTr = new Transform();
	spriteTr->SetPosition(Vector3(0.0f, 0.0f, 10.0f));
	spriteTr->SetScale(Vector3(6.0f, 6.0f, 3.0f));
	spriteObj->AddComponent(spriteTr);

	SpriteRenderer* sr = new SpriteRenderer();
	spriteObj->AddComponent(sr);

	std::shared_ptr<Material> spriteMaterial = ResourceManager::GetInstance()->Find<Material>(L"SpriteMaterial");

	/*Vector2 vec2(1.0f, 1.0f);
	spriteMaterial->SetData(eGPUParam::Vector2, &vec2);*/

	sr->SetMaterial(spriteMaterial);
	sr->SetMesh(mesh);

	mActiveScene->AddGameObject(spriteObj, eLayerType::Player);

	// HpBar
	GameObject*	HPbarObj = new GameObject();
	Transform* HPbarTr = new Transform();
	HPbarTr->SetPosition(Vector3(-5.0f, 3.0f, 10.0f));
	HPbarTr->SetScale(Vector3(1.0f, 1.0f, 1.0f));
	HPbarObj->AddComponent(HPbarTr);

	SpriteRenderer* HPbarsr = new SpriteRenderer();
	HPbarObj->AddComponent(HPbarsr);

	std::shared_ptr<Mesh> hpmesh = ResourceManager::GetInstance()->Find<Mesh>(L"RectMesh");
	std::shared_ptr<Material> HPbarMaterial = ResourceManager::GetInstance()->Find<Material>(L"UIMaterial");

	HPbarsr->SetMaterial(HPbarMaterial);
	HPbarsr->SetMesh(hpmesh);
	mActiveScene->AddGameObject(HPbarObj, eLayerType::UI);


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
