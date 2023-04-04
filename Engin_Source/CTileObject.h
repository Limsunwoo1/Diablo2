#pragma once
#include "CGameObject.h"

class TileObject : public GameObject
{
public:
	TileObject();
	virtual ~TileObject();

	virtual void Initalize();
	virtual void Update();
	virtual void FixedUpdate();
	virtual void Render();
private:

};

