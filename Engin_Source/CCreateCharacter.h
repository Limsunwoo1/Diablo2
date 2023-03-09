#pragma once
#include "CScene.h"

class CreateCharactorScene : public Scene
{
public:
	CreateCharactorScene();
	virtual ~CreateCharactorScene();

	virtual void Initalize();
	virtual void Update();
	virtual void FixedUpdate();
	virtual void Render();

	virtual void OnEnter();
	virtual void OnExit();
private:

};