#pragma once
#include "CGameObject.h"

typedef const std::vector<GameObject*>& GameObjects;

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
	const GameObjects GetGameObjects() { return mGameObjects; }

private:
	std::vector<GameObject*> mGameObjects;
};
