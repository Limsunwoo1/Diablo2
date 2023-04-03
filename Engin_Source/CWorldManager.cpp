#include "CWorldManager.h"
#define WORLD_SCALE 5000

WorldManager::WorldManager()
	: worldScale(WORLD_SCALE)
	, world{}
{

}

WorldManager::~WorldManager()
{

}

void WorldManager::Initialize()
{
	world.resize(WORLD_SCALE);

	for(int i = 0; i < WORLD_SCALE; ++i)
	{
		world[i].resize(WORLD_SCALE);
	}

	world[4999][4999] = 2;
}

HRESULT WorldManager::Load(const std::wstring& path)
{
	return E_NOTIMPL;
}

UINT WorldManager::TileNum(UINT x, UINT y)
{
	return  world[y][x];
}
