#pragma once
#include "CScene.h"

class MainTitleScene : public Scene
{
public:
	MainTitleScene();
	virtual ~MainTitleScene();

	virtual void Initalize();
	virtual void Update();
	virtual void FixedUpdate();
	virtual void Render();

	virtual void OnEnter();
	virtual void OnExit();


private:

};

