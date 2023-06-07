#pragma once
#include "CTileObject.h"

class TileCarveObject : public TileObject
{
public:
	TileCarveObject();
	~TileCarveObject();

	virtual void Update() override { /*GameObject::Update();*/ };
	virtual void Initalize() override;
private:

};

