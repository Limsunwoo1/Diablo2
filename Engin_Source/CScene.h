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

private:
	std::vector<Layer> mLayers;
};

