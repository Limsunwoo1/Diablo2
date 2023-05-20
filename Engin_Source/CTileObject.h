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
	Vector2 mSize;
	Vector2 mUV;

	int mPalletIndex;
	bool mbPass;

	int mArr[5][5];

	std::wstring mTexPath;
};

