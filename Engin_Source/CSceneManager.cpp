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

	mPlayScene->AddGameObject(cameraObj, eLayerType::Camera);

	// SMILE RECT
	GameObject* obj = new GameObject();
	Transform* tr = new Transform();
	tr->SetPosition(Vector3(0.0f, 0.0f, 20.0f));
	obj->AddComponent(tr);

	MeshRenderer* mr = new MeshRenderer();
	obj->AddComponent(mr);

	shared_ptr<Mesh> mesh = ResourceManager::GetInstance()->Find<Mesh>(L"RectMesh");
	shared_ptr<Material> material = ResourceManager::GetInstance()->Find<Material>(L"RectMaterial");

	Vector2 vec2(1.0f, 1.0f);
	material->SetData(eGpuParam::Vector2, &vec2);

	mr->SetMaterial(material.get());
	mr->SetMesh(mesh.get());

	PlayerScript* script = new PlayerScript();
	obj->AddComponent(script);

	shared_ptr<Texture>texture = ResourceManager::GetInstance()->Load<Texture>(L"SmileTexture", L"Smile.png");
	texture->BidShader(eShaderStage::PS, 0);

	mPlayScene->AddGameObject(obj, eLayerType::Player);
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
