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
	void Rlease();

	Scene* GetActiveScene() { return mActiveScene; }
private:
	Scene* mActiveScene;
};

