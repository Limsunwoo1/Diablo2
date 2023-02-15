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
	: mPlayScene(nullptr)
{
	
}
SceneManager::~SceneManager()
{
	
}

void SceneManager::Initalize()
{
	mPlayScene = new Scene();
	mPlayScene->Initalize();

	//Camera GameObject
	GameObject* cameraObj = new GameObject();
	Transform* cameraTr = new Transform();
	cameraTr->SetPosition(Vector3(0.0f, 0.0f, 0.0f));
	cameraObj->AddComponent(cameraTr);

	Camera* cameraComp = new Camera();
	cameraObj->AddComponent(cameraComp);

	CameraScript* cameraScript = new CameraScript();
	cameraObj->AddComponent(cameraScript);

	mPlayScene->AddGameObject(cameraObj, eLayerType::Camera);

	// SMILE RECT
	GameObject* obj = new GameObject();
	Transform* tr = new Transform();
	tr->SetPosition(Vector3(0.0f, 0.0f, 10.0f));
	tr->SetScale(Vector3(3.0f, 3.0f, 3.0f));
	obj->AddComponent(tr);

	MeshRenderer* mr = new MeshRenderer();
	obj->AddComponent(mr);

	shared_ptr<Mesh> mesh = ResourceManager::GetInstance()->Find<Mesh>(L"RectMesh");
	shared_ptr<Material> material = ResourceManager::GetInstance()->Find<Material>(L"RectMaterial");

	Vector2 vec2(1.0f, 1.0f);
	material->SetData(eGpuParam::Vector2, &vec2);

	mr->SetMaterial(material);
	mr->SetMesh(mesh);

	mPlayScene->AddGameObject(obj, eLayerType::Player);

	// SpriteObj
	GameObject* spriteObj = new GameObject();
	Transform* spriteTr = new Transform();
	spriteTr->SetPosition(Vector3(5.0f, 0.0f, 10.0f));
	spriteObj->AddComponent(spriteTr);

	SpriteRenderer* sr = new SpriteRenderer();
	spriteObj->AddComponent(sr);

	std::shared_ptr<Material> spriteMaterial = ResourceManager::GetInstance()->Find<Material>(L"SpriteMaterial");

	//Vector2 vec2(1.0f, 1.0f);
	//spriteMaterial->SetData(eGPUParam::Vector2, &vec2);

	sr->SetMaterial(spriteMaterial);
	sr->SetMesh(mesh);

	mPlayScene->AddGameObject(spriteObj, eLayerType::Player);
}

void SceneManager::Update()
{
	mPlayScene->Update();
}

void SceneManager::FixedUpdate()
{
	mPlayScene->FixedUpdate();
}

void SceneManager::Render()
{
	mPlayScene->Render();
}

void SceneManager::Rlease()
{
	delete mPlayScene;
	mPlayScene = nullptr;
}
