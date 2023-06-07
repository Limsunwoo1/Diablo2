#pragma once
#include "CGameObject.h"


using namespace std;

typedef std::map<pair<int, int>, vector<int>> UnMoveAbleMap;
typedef std::map<pair<int, int>, vector<class TileCarveObject*>> CarveObjects;
class TilePallet : public GameObject
{
public:
	TilePallet();
	virtual ~TilePallet();

	virtual void Initalize() override;
	virtual void Update() override;
	virtual void FixedUpdate() override;
	virtual void Render() override;

	void Load();
	void Load(const std::wstring& path, eSceneType type);
	void Save();

	void CreateTile(const wstring& key, eLayerType type, Pos_Data pos, Screen_IDX_Data screenIdx, IDX_Data uvIdx);
	void CreateTile(const wstring& key, eLayerType type, Pos_Data pos, Screen_IDX_Data screenIdx, IDX_Data uvIdx, eSceneType sceneType);
	void CreateWall(const wstring& key, eLayerType type, Pos_Data pos, Size_Data size, Offset_Data offset, Screen_IDX_Data screenIdx);
	void CreateWall(const wstring& key, eLayerType type, Pos_Data pos, Size_Data size, Offset_Data offset, Screen_IDX_Data screenIdx, eSceneType sceneType);

	void SetToolScene(class ToolScene* scene) { mToolScene = scene; }
	ToolScene* GetToolScene() { return mToolScene; }

	void InsertUnMoveAbleData(int x, int y, int idx, class TileObject* obj);
	void DeleteUnMoveAbleData(int x, int y, int idx);

	void InserCarveObject(int x, int y, int idx, class TileObject* obj);
	void DeleteCarveObject(int x, int y, int idx);

private:

	int mCol;
	int mRow;

	ToolScene* mToolScene;
	std::map<pair<int, int>, vector<int>> mUnMoveable_Data;
	std::map<pair<int, int>, vector<class TileCarveObject*>> mCarve_Object;
};

