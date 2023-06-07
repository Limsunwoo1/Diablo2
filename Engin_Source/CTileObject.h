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
	void SetScreenIndex(UINT x, UINT y) { mScreenIndexX = x, mScreenIndexY = y; }

	std::pair<UINT, UINT> GetMaxIndex() { return std::make_pair(mMaxX, mMaxY); }
	std::pair<UINT, UINT> GetIndex() { return std::make_pair(mIndexX, mIndexY); }
	std::pair<UINT, UINT> GetScreenIndex() { return std::make_pair(mScreenIndexX, mScreenIndexY); }

	class Material* GetMaterial() { return mMaterial; }
	void SetMaterial(class Material* mater) { mMaterial = mater; }

	const std::vector<int>& GetArr() { return mArr; }
	void SetArr(const std::vector<int>& arr) { mArr = arr; }

private:
	Vector2 mUV;

	UINT mMaxX;
	UINT mMaxY;

	UINT mIndexX;
	UINT mIndexY;

	UINT mScreenIndexX;
	UINT mScreenIndexY;

	bool mbPass;
	bool mbUpdate;
	bool mbOnTile;

	std::vector<int> mArr;
	int mArrIdx;
	std::wstring mTexPath;

	Material* mMaterial;
};

