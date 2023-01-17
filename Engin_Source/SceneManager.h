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
private:
	Scene* mPlayScene;
};

