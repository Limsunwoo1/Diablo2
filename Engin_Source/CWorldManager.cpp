#include "CWorldManager.h"
#include "CInput.h"
#include "CObjectManager.h"

#define WORLD_SCALE 1000

// 1 플레이어, 2 도작치, 3 장애물, 0 이동가능

WorldManager::WorldManager()
	: worldScale(WORLD_SCALE)
	, world{}
{
	world.resize(5);

	for (int i = 0; i < world.size(); ++i)
	{
		world[i].resize(5);
	}
}

WorldManager::~WorldManager()
{
	Player = nullptr;
	for (int i = 0; i < WORLD_SCALE; ++i)
	{
		world[i].clear();
	}
	world.clear();
}

void WorldManager::Initialize()
{
	world.resize(WORLD_SCALE);

	for(int i = 0; i < WORLD_SCALE; ++i)
	{
		world[i].resize(WORLD_SCALE);
	}

	world[10][10] = 1;

	PlayerIndex.x = 10;
	PlayerIndex.y = 10;
	//world[99][99] = 2;
}

void WorldManager::Update()
{
}

HRESULT WorldManager::Load(const std::wstring& path)
{
	return E_NOTIMPL;
}

UINT WorldManager::GetTileNum(const UINT& x, const UINT& y)
{
	return  world[y][x];
}

bool WorldManager::CheckPlayerIndex(const int& x, const int& y)
{
	if (x > worldScale -1 || y > worldScale -1)
		return false;

	if (x <  0 || y < 0)
		return false;

	if (world[y][x] == 3)
		return false;

	return true;
}

bool WorldManager::CheckEndIndex(const int& x, const int& y)
{
	if (x > worldScale -1 || y > worldScale -1)
		return false;

	if (x < 0 || y < 0)
		return false;

	if (world[y][x] == 3)
		return false;

	return true;
}

bool WorldManager::SetPath(const int& startX, const int& startY, const int& endX, const int& endY)
{
	Math::Vector2 playerTemp = PlayerIndex;
	Math::Vector2 endTemp = EndIndex;

	if (!CheckPlayerIndex(startX, startY) || !CheckEndIndex(endX, endY))
	{
		return false;
	}

	world[PlayerIndex.y][PlayerIndex.x] = 0;
	world[EndIndex.y][EndIndex.x] = 0;

	world[startY][startX] = 1;
	PlayerIndex.x = startX;
	PlayerIndex.y = startY;

	world[endY][endX] = 2;
	EndIndex.x = endX;
	EndIndex.y = endY;

	return true;
}

std::pair<int, int> WorldManager::GetTileIndex(Vector2 MousePos)
{
	auto idx = Input::GetInstance()->GetIsoMetricIDX(MousePos);
	TileObject* tile = ObjectManager::GetInstance()->GetTile(idx.first, idx.second);

	if (tile == nullptr)
		return pair(-1, -1);

	for (int i = 0; i < mWorldTileData.size(); ++i)
	{
		for (int j = 0; j < mWorldTileData[i].size(); ++j)
		{
			if (mWorldTileData[i][j] == tile)
				return std::pair(i,j);
		}
	}
	
	return pair(-1, -1);
}
