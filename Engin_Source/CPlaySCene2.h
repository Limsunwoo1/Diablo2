#pragma once
#include "CScene.h"


class PlayScene2 : public Scene
{
public:
	PlayScene2();
	virtual ~PlayScene2();

	virtual void Initalize();
	virtual void Update();
	virtual void FixedUpdate();
	virtual void Render();

	virtual void OnEnter();
	virtual void OnExit();

private:
};