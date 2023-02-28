#pragma once
#include "CEntity.h"
#include "CLayer.h"

class Scene : public Entity
{
public:
	Scene();
	virtual ~Scene();

	virtual void Initalize();
	virtual void Update();
	virtual void FixedUpdate();
	virtual void Render();
	virtual void Destroy();

	virtual void OnEnter(){};
	virtual void OnExit(){};

	void AddGameObject(GameObject* gameObj, const eLayerType type);
	Layer& GetLayer(eLayerType type) { return mLayers[(UINT)type]; }
	std::vector<GameObject*> GetDontDestroyObjects();
private:
	std::vector<Layer> mLayers;
};

