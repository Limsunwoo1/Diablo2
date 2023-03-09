#pragma once
#include "CScene.h"

class CharacterSelectScene : public Scene
{
public:
	CharacterSelectScene();
	virtual ~CharacterSelectScene();

	virtual void Initalize();
	virtual void Update();
	virtual void FixedUpdate();
	virtual void Render();

	virtual void OnEnter();
	virtual void OnExit();
private:

};