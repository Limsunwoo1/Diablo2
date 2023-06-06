#include "CTilePallet.h"
#include "CMeshRenderer.h"
#include "CResourceManager.h"
#include "CSceneManager.h"
#include "CTileObject.h"
#include "CObject.h"
#include "CWallObject.h"

TilePallet::TilePallet()
{
}

TilePallet::~TilePallet()
{
}

void TilePallet::Initalize()
{

}

void TilePallet::Update()
{
	GameObject::Update();
}

void TilePallet::FixedUpdate()
{
	GameObject::FixedUpdate();
}

void TilePallet::Render()
{
	GameObject::Render();
}

void TilePallet::Load()
{
	OPENFILENAME ofn = {};

	wchar_t szFilePath[256] = {};

	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;
	ofn.lpstrFile = szFilePath;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = 256;
	ofn.lpstrFilter = L"All\0*.*\0Text\0*.TXT\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	if (false == GetOpenFileName(&ofn))
		return;

	FILE* pFile = nullptr;
	_wfopen_s(&pFile, szFilePath, L"rb");
	if (pFile == nullptr)
		return;

	while (true)
	{
		UINT32 type;
		UINT32 count;
		string key = "";
		char* cKey = nullptr;
		Pos_Data Pos;
		Size_Data Size;
		Offset_Data Offset;
		Screen_IDX_Data screenIndex;
		IDX_Data IDx;

		if (fread(&type, sizeof(UINT32), 1, pFile) == NULL)
			break;

		if (type == (UINT32)eLayerType::Tile)
		{
			if (fread(&count, sizeof(UINT32), 1, pFile) == NULL)
				break;

			cKey = new char[count];
			if (fread(cKey, sizeof(char), count, pFile) == NULL)
			{
				delete cKey;
				break;
			}
			key.assign(cKey, count);

			if (fread(&Pos, sizeof(UINT64), 1, pFile) == NULL)
				break;

			if (fread(&screenIndex, sizeof(UINT64), 1, pFile) == NULL)
				break;

			if (fread(&IDx, sizeof(UINT64), 1, pFile) == NULL)
				break;

			CreateTile(wstring(key.begin(), key.end()), (eLayerType)type, Pos, screenIndex, IDx);
		}
		else if(type == (UINT32)eLayerType::Wall)
		{
			if (fread(&count, sizeof(UINT32), 1, pFile) == NULL)
				break;

			cKey = new char[count];
			if (fread(cKey, sizeof(char), count, pFile) == NULL)
			{
				delete cKey;
				break;
			}
			key.assign(cKey, count);

			if (fread(&Pos, sizeof(UINT64), 1, pFile) == NULL)
				break;

			if (fread(&Size, sizeof(UINT64), 1, pFile) == NULL)
				break;

			if (fread(&Offset, sizeof(UINT64), 1, pFile) == NULL)
				break;

			if (fread(&screenIndex, sizeof(UINT64), 1, pFile) == NULL)
				break;

			CreateWall(wstring(key.begin(), key.end()), (eLayerType)type, Pos, Size, Offset, screenIndex);
		}

		delete cKey;
	}

	fclose(pFile);
}

void TilePallet::Load(const std::wstring& path, eSceneType scenetype)
{
	FILE* pFile = nullptr;
	_wfopen_s(&pFile, path.c_str(), L"rb");
	if (pFile == nullptr)
		return;

	while (true)
	{
		UINT32 type;
		UINT32 count;
		string key = "";
		char* cKey = nullptr;
		Pos_Data Pos;
		Size_Data Size;
		Offset_Data Offset;
		Screen_IDX_Data screenIndex;
		IDX_Data IDx;

		if (fread(&type, sizeof(UINT32), 1, pFile) == NULL)
			break;

		if (type == (UINT32)eLayerType::Tile)
		{
			if (fread(&count, sizeof(UINT32), 1, pFile) == NULL)
				break;

			cKey = new char[count];
			if (fread(cKey, sizeof(char), count, pFile) == NULL)
			{
				delete cKey;
				break;
			}
			key.assign(cKey, count);

			if (fread(&Pos, sizeof(UINT64), 1, pFile) == NULL)
				break;

			if (fread(&screenIndex, sizeof(UINT64), 1, pFile) == NULL)
				break;

			if (fread(&IDx, sizeof(UINT64), 1, pFile) == NULL)
				break;

			CreateTile(wstring(key.begin(), key.end()), (eLayerType)type, Pos, screenIndex, IDx, scenetype);
		}
		else if (type == (UINT32)eLayerType::Wall)
		{
			if (fread(&count, sizeof(UINT32), 1, pFile) == NULL)
				break;

			cKey = new char[count];
			if (fread(cKey, sizeof(char), count, pFile) == NULL)
			{
				delete cKey;
				break;
			}
			key.assign(cKey, count);

			if (fread(&Pos, sizeof(UINT64), 1, pFile) == NULL)
				break;

			if (fread(&Size, sizeof(UINT64), 1, pFile) == NULL)
				break;

			if (fread(&Offset, sizeof(UINT64), 1, pFile) == NULL)
				break;

			if (fread(&screenIndex, sizeof(UINT64), 1, pFile) == NULL)
				break;

			CreateWall(wstring(key.begin(), key.end()), (eLayerType)type, Pos, Size, Offset, screenIndex, scenetype);
		}
		else
		{
			break;
		}

		delete cKey;
	}

	fclose(pFile);
}

void TilePallet::Save()
{
	// open a file name
	OPENFILENAME ofn = {};

	wchar_t szFilePath[256] = {};

	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;
	ofn.lpstrFile = szFilePath;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = 256;
	ofn.lpstrFilter = L"Tile\0*.tile\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	if (false == GetSaveFileName(&ofn))
		return;

	FILE* pFile = nullptr;
	_wfopen_s(&pFile, szFilePath, L"wb");
	if (pFile == nullptr)
		return;

	Scene* secne = SceneManager::GetInstance()->GetScene(eSceneType::Tool);

	std::vector<GameObject*> objects = secne->GetLayer(eLayerType::Tile).GetGameObjects();
	std::vector<GameObject*>::iterator iter;

	for (iter = objects.begin(); iter != objects.end(); ++iter)
	{
		TileObject* tile = dynamic_cast<TileObject*>((*iter));
		if (tile == nullptr)
			continue;

		UINT32 type = (UINT32)tile->GetLayerType();
		fwrite(&type, sizeof(UINT32), 1, pFile);

		BaseRenderer* renderer = tile->GetComponent<BaseRenderer>();
		Texture2D* tex = renderer->GetMaterial().lock()->GetTexture(eTextureSlot::T0).lock().get();
		const wstring& texKey = tex->GetKey();
		string biteSize = string(texKey.begin(), texKey.end());
		const char* cKey = biteSize.c_str();
		UINT32 count = biteSize.size();

		fwrite(&count, sizeof(UINT32), 1, pFile);
		fwrite(cKey, sizeof(char), count, pFile);

		Transform* tr = (*iter)->GetComponent<Transform>();
		Vector3 Pos = tr->GetPosition();
		Pos_Data pos;
		pos.posX = Pos.x;
		pos.posY = Pos.y;
		fwrite(&pos, sizeof(UINT64), 1, pFile);


		Screen_IDX_Data ScrrenID;
		auto Index = tile->GetScreenIndex();
		ScrrenID.screenIdxX = Index.first;
		ScrrenID.screenIdxY = Index.second;
		fwrite(&ScrrenID, sizeof(UINT64), 1, pFile);

		IDX_Data TileUV;
		auto uvIndex = tile->GetIndex();
		TileUV.idxX = uvIndex.first;
		TileUV.idxY = uvIndex.second;
		fwrite(&TileUV, sizeof(UINT64), 1, pFile);
	}

	SceneManager::GetInstance()->SortWallObject();
	std::vector<GameObject*> Wallobjects = secne->GetLayer(eLayerType::Wall).GetGameObjects();
	for (GameObject* object : Wallobjects)
	{
		WallObject* Wall = dynamic_cast<WallObject*>(object);
		if (Wall == nullptr)
			continue;

		UINT32 type = (UINT32)Wall->GetLayerType();
		fwrite(&type, sizeof(UINT32), 1, pFile);

		Texture2D* tex = Wall->GetTexture2D().lock().get();
		const wstring& name = tex->GetName();
		string biteSize = string(name.begin(), name.end());
		int count = biteSize.size();
		const char* cKey = biteSize.c_str();

		fwrite(&count, sizeof(UINT32), 1, pFile);
		fwrite(cKey, sizeof(char), count, pFile);

		Transform* WallTr = Wall->GetComponent<Transform>();
		Vector3 pos = WallTr->GetPosition();
		Pos_Data PosData;
		PosData.posX = pos.x;
		PosData.posY = pos.y;
		fwrite(&PosData, sizeof(UINT64), 1, pFile);

		Size_Data SizeData;
		Vector3 size = WallTr->GetSize();
		SizeData.sizeX = size.x;
		SizeData.sizeY = size.y;
		fwrite(&SizeData, sizeof(UINT64), 1, pFile);

		Offset_Data OffsetData;
		Vector2 offset = Wall->GetOffset();
		OffsetData.offsetX = offset.x;
		OffsetData.offsetY = offset.y;
		fwrite(&OffsetData, sizeof(UINT64), 1, pFile);
		
		Screen_IDX_Data ScreenIdx;
		auto ScreenIDx = Wall->GetScreenIDX();
		ScreenIdx.screenIdxX = ScreenIDx.first;
		ScreenIdx.screenIdxY = ScreenIDx.second;
		fwrite(&ScreenIdx, sizeof(UINT64), 1, pFile);
	}

	fclose(pFile);
}

void TilePallet::CreateTile(const wstring& key, eLayerType type, Pos_Data pos, Screen_IDX_Data screenIdx, IDX_Data uvIdx)
{
	std::weak_ptr<Texture2D> tex = ResourceManager::GetInstance()->Find<Texture2D>(key);
	
	Scene* scene = SceneManager::GetInstance()->GetScene(eSceneType::Tool);

	TileObject* tile = Object::Instantiate<TileObject>(eLayerType::Tile, scene);

	tile->SetScreenIndex(screenIdx.screenIdxX, screenIdx.screenIdxY);
	tile->SetMaxIndex(tex.lock()->GetMaxX(), tex.lock()->GetMaxY());
	tile->SetIndex(uvIdx.idxX, uvIdx.idxY);
	tile->GetMaterial()->SetTexture(eTextureSlot::T0, tex);

	Transform* tr = tile->GetComponent<Transform>();
	tr->SetPosition(Vector3(pos.posX, pos.posY, 50.f));
	tr->SetSize(Vector3(TILE_X_HALF_SIZE * 2, TILE_Y_HALF_SIZE * 2, 1.0f));
}

void TilePallet::CreateTile(const wstring& key, eLayerType type, Pos_Data pos, Screen_IDX_Data screenIdx, IDX_Data uvIdx, eSceneType sceneType)
{
	std::weak_ptr<Texture2D> tex = ResourceManager::GetInstance()->Find<Texture2D>(key);
	if (tex.lock() == nullptr)
		return;

	TileObject* tile = new TileObject();
	tile->Initalize();

	tile->SetScreenIndex(screenIdx.screenIdxX, screenIdx.screenIdxY);
	tile->SetMaxIndex(tex.lock()->GetMaxX(), tex.lock()->GetMaxY());
	tile->SetIndex(uvIdx.idxX, uvIdx.idxY);
	tile->GetMaterial()->SetTexture(eTextureSlot::T0, tex);

	Transform* tr = tile->GetComponent<Transform>();
	tr->SetPosition(Vector3(pos.posX, pos.posY, 50.f));
	tr->SetSize(Vector3(TILE_X_HALF_SIZE * 2, TILE_Y_HALF_SIZE * 2, 1.0f));

	Object::Instantiate<TileObject>(eLayerType::Tile, sceneType, tile);
}

void TilePallet::CreateWall(const wstring& key, eLayerType type, Pos_Data pos, Size_Data size, Offset_Data offset, Screen_IDX_Data screenIdx)
{
	std::weak_ptr<Texture2D> tex = ResourceManager::GetInstance()->Find<Texture2D>(key);
	if (tex.lock() == nullptr)
		return;

	Scene* scene = SceneManager::GetInstance()->GetScene(eSceneType::Tool);

	WallObject* wall = Object::Instantiate<WallObject>(eLayerType::Wall, scene);

	wall->SetTexture(tex);
	wall->SetOffset(Vector2(offset.offsetX, offset.offsetY));
	wall->SetScrrenIndex(screenIdx.screenIdxX, screenIdx.screenIdxY);
	
	Transform* tr = wall->GetComponent<Transform>();
	tr->SetPosition(Vector3(pos.posX, pos.posY, 49.f));
	tr->SetSize(Vector3(size.sizeX, size.sizeY, 1.0f));
}

void TilePallet::CreateWall(const wstring& key, eLayerType type, Pos_Data pos, Size_Data size, Offset_Data offset, Screen_IDX_Data screenIdx, eSceneType sceneType)
{
	std::weak_ptr<Texture2D> tex = ResourceManager::GetInstance()->Find<Texture2D>(key);
	if (tex.lock() == nullptr)
		return;

	WallObject* wall = new WallObject();

	wall->SetTexture(tex);
	wall->SetOffset(Vector2(offset.offsetX, offset.offsetY));
	wall->SetScrrenIndex(screenIdx.screenIdxX, screenIdx.screenIdxY);

	Transform* tr = wall->GetComponent<Transform>();
	tr->SetPosition(Vector3(pos.posX, pos.posY, 49.f));
	tr->SetSize(Vector3(size.sizeX, size.sizeY, 1.0f));

	Object::Instantiate<TileObject>(eLayerType::Wall, sceneType, wall);
}
