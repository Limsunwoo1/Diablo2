#include "SceneManager.h"
#include "CTransform.h"
#include "CRenderer.h"
#include "CMeshRenderer.h"

SceneManager::SceneManager()
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
	tr->SetPosition(Vector3(-0.4f, -0.4f, 0.0f));
	obj->AddComponent(tr);

	MeshRenderer* mr = new MeshRenderer();
	obj->AddComponent(mr);

	mr->SetMesh(Renderer::mesh);
	mr->SetShader(Renderer::shader);

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
