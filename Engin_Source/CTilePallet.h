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
	void Load(const std::wstring& path, eSceneType type);
	void Save();

	void CreateTile(const wstring& key, eLayerType type, Tile_POS_Data pos, TileScreen_IDX_Data screenIdx, Tile_IDX_Data uvIdx);
	void CreateTile(const wstring& key, eLayerType type, Tile_POS_Data pos, TileScreen_IDX_Data screenIdx, Tile_IDX_Data uvIdx, eSceneType sceneType);
private:
	vector<class TileObject> mTiles;

	int mCol;
	int mRow;
};

