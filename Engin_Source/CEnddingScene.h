#pragma once
#include "CScene.h"

class EnddingScene : public Scene
{
public:
	EnddingScene();
	virtual ~EnddingScene();

	virtual void Initalize();
	virtual void Update();
	virtual void FixedUpdate();
	virtual void Render();
	virtual void Destroy();

	virtual void OnEnter();
	virtual void OnExit();
private:

};

