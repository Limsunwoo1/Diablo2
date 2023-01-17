#pragma once
#include "CGameObject.h"

class Layer
{
public:
	Layer();
	~Layer();

	virtual void Initalize();
	virtual void Update();
	virtual void FixedUpdate();
	virtual void Render();


private:
	std::vector<GameObject*> mGameObjects;
};

