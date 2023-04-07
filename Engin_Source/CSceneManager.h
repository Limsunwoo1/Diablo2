#pragma once
#include "CScene.h"

class SceneManager
{
	SINGLE(SceneManager)
public:
	void Initalize();
	void Update();
	void FixedUpdate();
	void Render();
	void Instantiate();
	void Destroy();
	void Rlease();

	void AddEvent(GameObject* obj) { mInsertObject.push(obj); }

	void LoadScene(eSceneType type);
	Scene* GetActiveScene() { return mActiveScene; }
private:
	std::vector<Scene*> mScenes;
	Scene* mActiveScene;
	std::queue<GameObject*> mInsertObject;
};

