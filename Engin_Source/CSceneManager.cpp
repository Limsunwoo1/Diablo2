#include "CSceneManager.h"
#include "CTitleScene.h"
#include "CPlayScene.h"

SceneManager::SceneManager()
	: mActiveScene(nullptr)
{
	
}
SceneManager::~SceneManager()
{
	
}

void SceneManager::Initalize()
{
	mScenes.resize((UINT)eSceneType::End);

	mScenes[(UINT)eSceneType::Title] = new TitleScene();
	mScenes[(UINT)eSceneType::Play] = new PlayScene();

	mActiveScene = mScenes[(UINT)eSceneType::Title];

	for (Scene* scene : mScenes)
	{
		scene->Initalize();
	}
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

void SceneManager::Destroy()
{
	mActiveScene->Destroy();
}

void SceneManager::Rlease()
{
	for (Scene* scene : mScenes)
	{
		delete scene;
		scene = nullptr;
	}
}

void SceneManager::LoadScene(eSceneType type)
{
	if (mActiveScene)
		mActiveScene->OnExit();

	// 바뀔때 DontDestroy 오브젝트는 다음씬으로 같이 넘겨줘야함.
	std::vector<GameObject*> gameObjs = mActiveScene->GetDontDestroyObjects();
	mActiveScene = mScenes[(UINT)type];

	for (GameObject* obj : gameObjs)
	{
		eLayerType type = obj->GetLayerType();
		mActiveScene->AddGameObject(obj, type);
	}

	mActiveScene->OnEnter();
}