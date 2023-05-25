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

	void SetMaxIndex(UINT x, UINT y) { mMaxX = x, mMaxY = y; }
	void SetIndex(UINT x, UINT y) { mIndexX = x, mIndexY = y; }

	std::pair<UINT, UINT> GetMaxIndex() { return std::make_pair(mMaxX, mMaxY); }
	std::pair<UINT, UINT> GetIndex() { return std::make_pair(mIndexX, mIndexY); }

	Material* GetMaterial() { return mMaterial; }
	void SetMaterial(Material* mater) { mMaterial = mater; }
private:
	Vector2 mUV;

	UINT mMaxX;
	UINT mMaxY;

	UINT mIndexX;
	UINT mIndexY;

	bool mbPass;
	bool mbUpdate;

	int mArr[5][5];
	std::wstring mTexPath;

	Material* mMaterial;
};

