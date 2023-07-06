#pragma once
#include "CEngine.h"
#include "CGameObject.h"
#include "CSkil.h"
#include "CInput.h"
#include "CTileObject.h"
#include "CWallObject.h"
#include <WinSock2.h>

using namespace std;

typedef std::map<std::pair<int, int>, TileObject*> TileObjectsIter;
typedef std::map<std::pair<int, int>, WallObject*> WallObjectsIter;
using OtherPlayer = std::unordered_map<SOCKET, GameObject*>;

class ObjectManager
{
	SINGLE(ObjectManager);
public:

	void Update();
	void FixedUpdate();
	void Initialize();
	void ResetWorld();
	void Realse();

	// 카메라의 Transparnets 렌더 부분에서 렌더링
	std::vector<GameObject*> GetTileRenderObject();
	std::vector<GameObject*> GetWallRenderObejct();

	void AddSKilObject(GameObject* obj)
	{
		Skil* skil = dynamic_cast<Skil*>(obj);
		if (!skil)
			return;

		skil->Death();
		SkilContainer.push(skil);
	}
	Skil* GetSkilObj(Player* owner);

	void SetOffsetData(eWallType type, Vector2 offset);
	Vector2 GetOffsetData(eWallType type);

	void SetSizeData(eWallType type, Vector2 size);
	Vector2 GetSizeData(eWallType type);

	void InsertTileObject(TileObject* tile);
	void InsertWallObject(WallObject* wall);

	void DeleteTileObject(TileObject* tile);
	void DeleteWallObjet(WallObject* wall);

	void LateUpdate();

	void Clear();

	TileObject* GetTile(int x, int y);

	GameObject* GetMonster(int x, int y);

	const std::map<std::pair<int, int>, TileObject*>& GetTileObjects() { return mTileObjects; };
	const 	std::map<std::pair<int, int>, WallObject*>& GetWallObjects() { return mWallObjects; };

	GameObject* PushOtherSocket(SOCKET sock);
	void DeleteOtherSocket(SOCKET sock);

	GameObject* GetOtherSocker(SOCKET sock);

	void SetOtherPos(SOCKET sock, Vector3 pos);

	const OtherPlayer& GetOthers() const { return mOthers; }
private:
	queue<Skil*> SkilContainer;
	vector<Vector2> ObjectOffsetData;
	vector<Vector2> ObjectSizeData;

	std::map<std::pair<int, int>, TileObject*> mTileObjects;
	std::map<std::pair<int, int>, WallObject*> mWallObjects;

	std::map<std::pair<int, int>, GameObject*> mMonsters;

	std::vector<GameObject*> deleteObjects;

	std::vector<GameObject*> mTiles;
	std::vector<GameObject*> mWalls;

	std::pair<int, int> mCurIDX;

	OtherPlayer mOthers;
};

