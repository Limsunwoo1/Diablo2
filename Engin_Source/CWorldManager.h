#pragma once
#include "CEngine.h"

using namespace std;
class WorldManager
{
	SINGLE(WorldManager);
public:
	void Initialize();
	HRESULT Load(const std::wstring& path);
	
	UINT GetTileNum(const UINT& x, const UINT& y);
	UINT GetScale() { return worldScale; }

private:
	UINT worldScale;

	vector<vector<int>> world;
};

