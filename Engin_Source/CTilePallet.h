#pragma once
#include "CGameObject.h"


using namespace std;

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
private:

	int mCol;
	int mRow;
};

