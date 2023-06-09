#pragma once
#include "CEngine.h"
#include "Math.h"

using namespace std;
class WorldManager
{
	SINGLE(WorldManager);
public:
	void Initialize();
	void Update();

	HRESULT Load(const std::wstring& path);
	
	UINT GetTileNum(const UINT& x, const UINT& y);
	UINT GetScale() { return worldScale; }

	bool CheckPlayerIndex(const int& x, const int& y);
	bool CheckEndIndex(const int& x, const int& y);


	void SetPlayerIndex(const int& x, const int& y) { PlayerIndex = Math::Vector2(x, y); world[y][x] = 1; }
	void SetEndIndex(const int& x, const int& y) { EndIndex = Math::Vector2(x, y); world[y][x] = 2; }
	void SetZero(const int& x, const int& y) { world[y][x] = 0; }
	void SetObstacle(const int& x, const int& y) { world[y][x] = 3; }

	bool SetPath(const int& startX, const int& startY, const int& endX, const int& endY);

	Math::Vector2 GetPlayerIndex() { return PlayerIndex; }
	Math::Vector2 GetEndIndex() { return EndIndex; }

	GameObject* GetPlayer() { return Player; }
	void SetPlayer(GameObject* player) { Player = player; }

	void PushWorldTileData(vector<vector<class TileObject*>> data) { mWorldTileData = data; }
	std::vector<vector<TileObject*>> DropWordTileData() { return mWorldTileData; }

	int GetTileDataWorldSize() {return mWorldTileData.size();}
	std::pair<int, int> GetTileIndex(Vector2 MousePos);
private:
	UINT worldScale;

	Math::Vector2 PlayerIndex;
	Math::Vector2 EndIndex;

	vector<vector<int>> world;
	vector<vector<class TileObject*>> mWorldTileData;
	class GameObject* Player;
};

