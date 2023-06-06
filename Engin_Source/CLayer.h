#pragma once
#include "CGameObject.h"

typedef const std::vector<GameObject*>& GameObjects;
typedef std::vector<GameObject*>::iterator GameObjectIter;

class Layer : public Entity
{
public:
	Layer();
	~Layer();

	virtual void Initalize();
	virtual void Update();
	virtual void FixedUpdate();
	virtual void Render();
	virtual void Destroy();

	void AddGameObject(GameObject* gameObject);
	const GameObjects GetGameObjects() { return mGameObjects; }
	std::vector<GameObject*> GetDontDestroyGameObjects();
private:
	std::vector<GameObject*> mGameObjects;
	bool mbSortTile;
};
