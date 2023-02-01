#pragma once
#include "CEntity.h"
#include "CLayer.h"

class Scene : public Entity
{
public:
	Scene();
	~Scene();

	virtual void Initalize();
	virtual void Update();
	virtual void FixedUpdate();
	virtual void Render();

	void AddGameObject(GameObject* gameObj, const eLayerType type);
private:
	std::vector<Layer> mLayers;
};

