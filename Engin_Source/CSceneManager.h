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
	void Destroy();
	void Rlease();

	void LoadScene(eSceneType type);
	Scene* GetActiveScene() { return mActiveScene; }
private:
	std::vector<Scene*> mScenes;
	Scene* mActiveScene;
};

