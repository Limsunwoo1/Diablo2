#pragma once
#include "CGameObject.h"

class Layer : public Entity
{
public:
	Layer();
	~Layer();

	virtual void Initalize();
	virtual void Update();
	virtual void FixedUpdate();
	virtual void Render();

	void AddGameObject(GameObject* gameObject);

private:
	std::vector<GameObject*> mGameObjects;
};

