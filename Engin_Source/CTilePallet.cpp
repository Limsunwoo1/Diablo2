#include "CTilePallet.h"
#include "CMeshRenderer.h"
#include "CResourceManager.h"
#include "CSceneManager.h"
#include "CTileObject.h"
#include "CObject.h"

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
		Tile_POS_Data Pos;
		TileScreen_IDX_Data screenIndex;
		Tile_IDX_Data IDx;

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

			delete cKey;
		}
	}

	fclose(pFile);
}

void TilePallet::Load(const std::wstring& path, eSceneType type)
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
		char* ckey = nullptr;
		Tile_POS_Data Pos;
		TileScreen_IDX_Data screenIndex;
		Tile_IDX_Data IDx;

		if (fread(&type, sizeof(UINT32), 1, pFile) == NULL)
			break;

		if (fread(&count, sizeof(UINT32), 1, pFile) == NULL)
			break;

		if (type == (UINT32)eLayerType::Tile)
		{
			
			if (fread(ckey, sizeof(char) , (UINT32)count , pFile) == NULL)
				break;

			if (fread(&Pos, sizeof(UINT64), 1, pFile) == NULL)
				break;

			if (fread(&screenIndex, sizeof(UINT64), 1, pFile) == NULL)
				break;

			if (fread(&IDx, sizeof(UINT64), 1, pFile) == NULL)
				break;

			key = ckey;
			CreateTile(wstring(key.begin(), key.end()), (eLayerType)type, Pos, screenIndex, IDx);
		}
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
		Tile_POS_Data pos;
		pos.posX = Pos.x;
		pos.posY = Pos.y;
		fwrite(&pos, sizeof(UINT64), 1, pFile);


		TileScreen_IDX_Data ScrrenID;
		auto Index = tile->GetScreenIndex();
		ScrrenID.screenIdxX = Index.first;
		ScrrenID.screenIdxY = Index.second;
		fwrite(&ScrrenID, sizeof(UINT64), 1, pFile);

		Tile_IDX_Data TileUV;
		auto uvIndex = tile->GetIndex();
		TileUV.idxX = uvIndex.first;
		TileUV.idxY = uvIndex.second;
		fwrite(&TileUV, sizeof(UINT64), 1, pFile);
	}

	fclose(pFile);
}

void TilePallet::CreateTile(const wstring& key, eLayerType type, Tile_POS_Data pos, TileScreen_IDX_Data screenIdx, Tile_IDX_Data uvIdx)
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

void TilePallet::CreateTile(const wstring& key, eLayerType type, Tile_POS_Data pos, TileScreen_IDX_Data screenIdx, Tile_IDX_Data uvIdx, eSceneType sceneType)
{
	std::weak_ptr<Texture2D> tex = ResourceManager::GetInstance()->Find<Texture2D>(key);

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
