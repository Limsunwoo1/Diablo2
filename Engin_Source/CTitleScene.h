#pragma once
#include "CScene.h"

class TitleScene : public Scene
{
public:
	TitleScene();
	virtual ~TitleScene();

	virtual void Initalize();
	virtual void Update();
	virtual void FixedUpdate();
	virtual void Render();

	virtual void OnEnter();
	virtual void OnExit();

private:
};