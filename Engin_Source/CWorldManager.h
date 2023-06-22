#pragma once
#include "CEngine.h"
#include "Math.h"
#include "CGameObject.h"

using namespace std;
using namespace Math;
class WorldManager
{
	SINGLE(WorldManager);
public:
	void Initialize();
	void Update();

	HRESULT Load(const std::wstring& path);

	UINT GetScale() { return worldScale; }

	GameObject* GetPlayer() { return Player; }
	void SetPlayer(GameObject* player) { Player = player; }

	void PushWorldTileData(vector<vector<class TileObject*>>& data);
	std::vector<vector<TileObject*>> DropWordTileData() { return mWorldTileData; }
	vector<vector<TileCarveData>> DropWolrdTileCarveData() { return mWolrdTileCarveData; }
	void SetWorldTileCarveData(int x, int y, TileObject* tile);

	int GetTileDataWorldSize() { return mWorldTileData.size(); }
	std::pair<int, int> GetTileIndex(Vector2 MousePos);
private:
	UINT worldScale;

	vector<vector<class TileObject*>> mWorldTileData;
	vector<vector<TileCarveData>> mWolrdTileCarveData;
	class GameObject* Player;
};