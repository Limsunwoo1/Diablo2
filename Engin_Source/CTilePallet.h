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

	void Load();
	void Save();

private:
	vector<class TileObject> mTiles;

	int mCol;
	int mRow;
};

