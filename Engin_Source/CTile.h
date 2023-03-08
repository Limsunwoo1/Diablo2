#pragma once
#include "CGameObject.h"

class Tile : public GameObject
{
public:
	Tile();
	virtual ~Tile();

	virtual void Initalize();
	virtual void Update();
	virtual void FixedUpdate();
	virtual void Render();

private:
	UINT mIndex;
};

