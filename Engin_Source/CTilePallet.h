#pragma once
#include "CGameObject.h"


using namespace std;

class TilePallet : public GameObject
{
public:
	TilePallet();
	virtual ~TilePallet();

	virtual void Initalize() override;
	virtual void Update() override;
	virtual void FixedUpdate() override;
	virtual void Render() override;


private:
	vector<class TileObject> mTiles;
	Vector2 mGridSize;

	int mCol;
	int mRow;
};

