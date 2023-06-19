#include "CWorldManager.h"
#include "CInput.h"
#include "CObjectManager.h"
#include "CTileObject.h"
#include "CRenderer.h"
#include "CCamera.h"

#define WORLD_SCALE 1000

// 1 플레이어, 2 도작치, 3 장애물, 0 이동가능

WorldManager::WorldManager()
	: worldScale(WORLD_SCALE)
{
}

WorldManager::~WorldManager()
{
	if (Player)
		delete Player;

	Player = nullptr;
}

void WorldManager::Initialize()
{
}

void WorldManager::Update()
{
}

HRESULT WorldManager::Load(const std::wstring& path)
{
	return E_NOTIMPL;
}

void WorldManager::PushWorldTileData(vector<vector<class TileObject*>> data)
{
	mWorldTileData = data;

	// 타일내부를 4등분 했기에 [2][2] 배열 크기로 계산한다
	int vecSize = mWorldTileData.size() * 2;

	mWolrdTileCarveData.resize(vecSize);
	for (int i = 0; i < mWolrdTileCarveData.size(); ++i)
	{
		mWolrdTileCarveData[i].resize(vecSize);
	}

	for (int i = 0; i < mWorldTileData.size(); ++i)
	{
		for (int j = 0; j < mWorldTileData.size(); ++j)
		{
			if (mWorldTileData[i][j] == nullptr)
				continue;

			
			SetWorldTileCarveData(j, i, mWorldTileData[i][j]);
		}
	}

}

void WorldManager::SetWorldTileCarveData(int x, int y, TileObject* tile)
{
	if (tile == nullptr)
		return;

	TileCarveData data = {};
	data.TileIdxX = tile->GetScreenIndex().first;
	data.TileIdxY = tile->GetScreenIndex().second;
	data.tile = tile;
	vector<int>  TileArr = tile->GetArr();

	int arrIdx[4] = { 2,3,0,1 };
	//int arrIdx[4] = { 0,1,2,3 };
	int count = 0;
	for (int i = y * 2; i <= y * 2 + 1; ++i)
	{
		for (int j = x * 2; j <= x * 2 + 1; ++j)
		{
			data.ArrValue = TileArr[arrIdx[count]];

			mWolrdTileCarveData[i][j] = data;
			count++;
		}
	}

	int a = 0;
}

std::pair<int, int> WorldManager::GetTileIndex(Vector2 MousePos)
{
	auto idx = Input::GetInstance()->GetIsoMetricIDX(MousePos);
	TileObject* tile = ObjectManager::GetInstance()->GetTile(idx.first, idx.second);

	Vector3 pos = Renderer::mainCamera->GetOwner()->GetComponent<Transform>()->GetPosition();

	auto mainidx = Input::GetInstance()->GetIsoMetricIDX(Vector2(pos.x, pos.y));

	if (tile == nullptr)
		return pair(-1, -1);

	for (int i = 0; i < mWorldTileData.size(); ++i)
	{
		for (int j = 0; j < mWorldTileData[i].size(); ++j)
		{
			if (mWorldTileData[i][j] == tile)
				return std::pair(j, i);
		}
	}

	return pair(-1, -1);
}