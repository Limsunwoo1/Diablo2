#include "CSceneManager.h"
#include "CTransform.h"
#include "CRenderer.h"
#include "CResourceManager.h"
#include "CMeshRenderer.h"
#include "CShader.h"
#include "CMesh.h"
#include "CTexture.h"

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

	GameObject* obj = new GameObject();
	Transform* tr = new Transform();
	//tr->SetPosition(Vector3(-0.4f, -0.4f, 0.0f));
	obj->AddComponent(tr);

	MeshRenderer* mr = new MeshRenderer();
	obj->AddComponent(mr);

	Shader* shader = ResourceManager::GetInstance()->Find<Shader>(L"RectShader");
	Mesh* mesh = ResourceManager::GetInstance()->Find<Mesh>(L"RectMesh");

	mr->SetShader(shader);
	mr->SetMesh(mesh);

	Texture* texture = ResourceManager::GetInstance()->Load<Texture>(L"SmileTexture", L"Smile.png");
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
