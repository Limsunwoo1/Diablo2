#include "CObjectManager.h"
#include "CInput.h"
#include "CRenderer.h"
#include "CCamera.h"
#include "CSceneManager.h"
#include "CLayer.h"
#include "CWorldManager.h"
#include "CObject.h"
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

	mCurIDX = std::pair(-100, -100);
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

	for (auto tile : mTileObjects)
	{
		if (tile.second == nullptr)
			continue;

		delete tile.second;
		tile.second = nullptr;
	}

	for (auto Wall : mWallObjects)
	{
		if (Wall.second == nullptr)
			continue;

		delete Wall.second;
		Wall.second = nullptr;
	}
}

void ObjectManager::Update()
{
	if (Renderer::mainCamera == nullptr)
		return;

	ResetWorld();
}

void ObjectManager::FixedUpdate()
{
}

void ObjectManager::Initialize()
{
}

void ObjectManager::ResetWorld()
{
	Transform* tr = Renderer::mainCamera->GetOwner()->GetComponent<Transform>();
	Vector3 pos = tr->GetPosition();

	auto idx = Input::GetInstance()->GetIsoMetricIDX(Vector2(pos.x, pos.y));

	//if (mCurIDX == idx)
	//	return;

	mCurIDX = idx;

	mTiles.clear();
	mWalls.clear();

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
	int worldY = 9;

	for (int i = startIdxY; i < endIdxY; ++i)
	{
		if (worldY < 0)
			break;

		for (int j = startIdxX; j < endIdxX; ++j)
		{
			if (i < 0 || j < 0)
			{
				worldX++;
				continue;
			}

			worldTileData[worldY][worldX] = nullptr;
			TileObjectsIter::iterator Tileiter =
				mTileObjects.find(std::pair(j, i));


			if (Tileiter != mTileObjects.end())
			{
				mTiles.emplace_back(Tileiter->second);
				worldTileData[worldY][worldX] = Tileiter->second;
			}

			/*WallObjectsIter::iterator Walliter =
				mWallObjects.find(std::pair(j, i));

			if (Walliter != mWallObjects.end())
				mWalls.emplace_back(Walliter->second);*/

			worldX++;
		}
		worldY--;
		worldX = 0;
	}

	WorldManager::GetInstance()->PushWorldTileData(worldTileData);

	for (int i = startIdxY - 2; i < endIdxY + 2; ++i)
	{
		for (int j = startIdxX - 2; j < endIdxX + 2; ++j)
		{

			WallObjectsIter::iterator Walliter =
				mWallObjects.find(std::pair(j, i));

			if (Walliter != mWallObjects.end())
				mWalls.emplace_back(Walliter->second);
		}
	}

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

			Vector3 aPos = aTr->GetPosition() + aTr->GetOffset() - (aTr->GetSize() * 0.5f);
			Vector3 bPos = bTr->GetPosition() + bTr->GetOffset() - (bTr->GetSize() * 0.5f);

			if (aPos.y == bPos.y)
				return a < b;
			else
				return aPos.y > bPos.y;
		});
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
	{
		delete tile;
		tile = nullptr;

		
		return;
	}

	mTileObjects.insert(std::make_pair(idx, tile));
	tile->FixedUpdate();
}

void ObjectManager::InsertWallObject(WallObject* wall)
{
	auto idx = wall->GetScreenIDX();

	WallObjectsIter::iterator iter = mWallObjects.find(std::pair(idx));

	if (iter != mWallObjects.end())
	{
		delete wall;
		wall = nullptr;


		return;
	}

	mWallObjects.insert(std::make_pair(idx, wall));
	wall->FixedUpdate();
}

void ObjectManager::DeleteTileObject(TileObject* tile)
{
	auto idx = tile->GetScreenIndex();

	TileObjectsIter::iterator iter = mTileObjects.find(std::pair(idx));

	if (iter != mTileObjects.end())
	{
		deleteObjects.push_back(tile);
		mTileObjects.erase(iter);
	}
}

void ObjectManager::DeleteWallObjet(WallObject* wall)
{
	auto idx = wall->GetScreenIDX();

	WallObjectsIter::iterator iter = mWallObjects.find(std::pair(idx));

	if (iter != mWallObjects.end())
	{
		deleteObjects.push_back(wall);
		mWallObjects.erase(iter);
	}
}

void ObjectManager::LateUpdate()
{
	for (auto* obj : deleteObjects)
	{
		if (obj == nullptr)
			continue;

		delete obj;
		obj = nullptr;
	}

	deleteObjects.clear();
}

TileObject* ObjectManager::GetTile(int x, int y)
{
	TileObjectsIter::iterator iter = mTileObjects.find(std::pair(x,y));

	if (iter == mTileObjects.end())
	{
		return nullptr;
	}

	return iter->second;
}

GameObject* ObjectManager::GetMonster(int x, int y)
{
	std::map<pair<int, int>, GameObject*>::iterator iter;
	iter = mMonsters.find(pair(x, y));

	if (iter != mMonsters.end())
		return iter->second;

	return nullptr;
}
