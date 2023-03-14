#pragma once
#include "CScene.h"


class PlayScene : public Scene
{
public:
	PlayScene();
	virtual ~PlayScene();

	virtual void Initalize();
	virtual void Update();
	virtual void FixedUpdate();
	virtual void Render();

	virtual void OnEnter();
	virtual void OnExit();

private:
};