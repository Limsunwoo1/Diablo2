#include "CObjectManager.h"
#include "CInput.h"
#include "CRenderer.h"
#include "CCamera.h"
#include "CSceneManager.h"
#include "CLayer.h"
#include "CWorldManager.h"

ObjectManager::ObjectManager()
{
	for (UINT i = 0; i < (UINT)eWallType::End; ++i)
	{
		ObjectOffsetData.emplace_back(Vector2::Zero);
	}

	for (UINT i = 0; i < (UINT)eWallType::End; ++i)
	{
		ObjectSizeData.emplace_back(Vector2(600.f, 600.f));
	}
}

ObjectManager::~ObjectManager()
{
	while (!SkilContainer.empty())
	{
		GameObject* obj = SkilContainer.front();
		SkilContainer.pop();

		if (obj == nullptr)
			continue;

		delete obj;
	}
}

void ObjectManager::Update()
{
	if (Renderer::mainCamera == nullptr)
		return;

	mTiles.clear();
	mWalls.clear();

	Transform* tr = Renderer::mainCamera->GetOwner()->GetComponent<Transform>();
	Vector3 pos = tr->GetPosition();

	auto idx = Input::GetInstance()->GetIsoMetricIDX(Vector2(pos.x, pos.y));

	// 화면 중앙을 기준으로 상하좌우 총 25 개만 파이프라인에 속함
	int startIdxX = idx.first - 5;
	int startIdxY = idx.second - 5;

	int endIdxX = idx.first + 5;
	int endIdxY = idx.second + 5;

	std::vector<std::vector<TileObject*>> worldTileData = {};
	worldTileData.resize(abs(endIdxY - startIdxY));

	for (int i = 0; i < worldTileData.size(); ++i)
	{
		worldTileData[i].resize(abs(endIdxY - startIdxY));
	}
	//std::vector<TileObject*> worldDataTile = ;

	int worldX = 0;
	int worldY = 0;

	for (int i = startIdxY; i < endIdxY; ++i)
	{
		for (int j = startIdxX; j < endIdxX; ++j)
		{
			worldTileData[worldY][worldX] = nullptr;
			if (i < 0 || j < 0)
			{
				worldX++;
				continue;
			}

			TileObjectsIter::iterator Tileiter =
				mTileObjects.find(std::pair(j, i));

			if (Tileiter != mTileObjects.end())
			{
				mTiles.emplace_back(Tileiter->second);
				worldTileData[worldY][worldX] = Tileiter->second;
			}

			WallObjectsIter::iterator Walliter =
				mWallObjects.find(std::pair(j, i));

			if (Walliter != mWallObjects.end())
				mWalls.emplace_back(Walliter->second);

			worldX++;
		}
		worldY++;
		worldX = 0;
	}

	WorldManager::GetInstance()->PushWorldTileData(worldTileData);

	std::sort(mTiles.begin(), mTiles.end(), [](GameObject* a, GameObject* b)
		{
			Transform* aTr = a->GetComponent<Transform>();
			Transform* bTr = b->GetComponent<Transform>();

			Vector3 aPos = aTr->GetPosition() + aTr->GetOffset();
			Vector3 bPos = bTr->GetPosition() + bTr->GetOffset();

			if (aPos.y == bPos.y)
				return a < b;
			else
				return aPos.y > bPos.y;
		});

	std::sort(mWalls.begin(), mWalls.end(), [](GameObject* a, GameObject* b)
		{
			Transform* aTr = a->GetComponent<Transform>();
			Transform* bTr = b->GetComponent<Transform>();

			Vector3 aPos = aTr->GetPosition();
			Vector3 bPos = bTr->GetPosition();

			if (aPos.y == bPos.y)
				return a < b;
			else
				return aPos.y > bPos.y;
		});
}

void ObjectManager::FixedUpdate()
{
}

void ObjectManager::Initialize()
{
}

std::vector<GameObject*> ObjectManager::GetTileRenderObject()
{
	std::vector<GameObject*> returnVec = mTiles;
	return returnVec;
}

std::vector<GameObject*> ObjectManager::GetWallRenderObejct()
{
	std::vector<GameObject*> returnVec = mWalls;
	return returnVec;
}


Skil* ObjectManager::GetSkilObj(Player* owner)
{
	Skil* out = nullptr;

	if (SkilContainer.empty())
	{
		out = new Skil(owner);
	}
	else
	{
		out = SkilContainer.front();
		SkilContainer.pop();

		out->Active();

		out->SetOwner(owner);
	}

	return out;
}

void ObjectManager::SetOffsetData(eWallType type, Vector2 offset)
{
	if (type == eWallType::End)
		return;

	ObjectOffsetData[(UINT)type] = offset;
}

Vector2 ObjectManager::GetOffsetData(eWallType type)
{
	if (type == eWallType::End)
		return Vector2::Zero;

	return ObjectOffsetData[(UINT)type];
}

void ObjectManager::SetSizeData(eWallType type, Vector2 size)
{
	if (type == eWallType::End)
		return;

	ObjectSizeData[(UINT)type] = size;
}

Vector2 ObjectManager::GetSizeData(eWallType type)
{
	if (type == eWallType::End)
		return Vector2(100.f,100.f);

	return ObjectSizeData[(UINT)type];
}

void ObjectManager::InsertTileObject(TileObject* tile)
{
	auto idx = tile->GetScreenIndex();
	
	TileObjectsIter::iterator iter = mTileObjects.find(std::pair(idx));

	if (iter != mTileObjects.end())
		return;

	mTileObjects.insert(std::make_pair(idx, tile));
}

void ObjectManager::InsertWallObject(WallObject* wall)
{
	auto idx = wall->GetScreenIDX();

	WallObjectsIter::iterator iter = mWallObjects.find(std::pair(idx));

	if (iter != mWallObjects.end())
		return;

	mWallObjects.insert(std::make_pair(idx, wall));
}

void ObjectManager::DeleteTileObject(TileObject* tile)
{
	auto idx = tile->GetScreenIndex();

	TileObjectsIter::iterator iter = mTileObjects.find(std::pair(idx));

	if (iter != mTileObjects.end())
	{
		mTileObjects.erase(iter);
		return;
	}
}

void ObjectManager::DeleteWallObjet(WallObject* wall)
{
	auto idx = wall->GetScreenIDX();

	WallObjectsIter::iterator iter = mWallObjects.find(std::pair(idx));

	if (iter != mWallObjects.end())
	{
		mWallObjects.erase(iter);
		return;
	}
}
