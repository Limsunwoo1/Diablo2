#include "CWorldManager.h"
#define WORLD_SCALE 100

WorldManager::WorldManager()
	: worldScale(WORLD_SCALE)
	, world{}
{

}

WorldManager::~WorldManager()
{
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

	world[99][99] = 2;
}

HRESULT WorldManager::Load(const std::wstring& path)
{
	return E_NOTIMPL;
}

UINT WorldManager::TileNum(const UINT& x, const UINT& y)
{
	return  world[y][x];
}
