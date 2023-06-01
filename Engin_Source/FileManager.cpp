#include "FileManager.h"
#include "CWorldManager.h"
#include "CApplication.h"
#include "Cplayer.h"
#include "FileManager.h"
#include "CItemManager.h"

#include <iostream>

extern CApplication Application;


namespace fs = std::filesystem;

FileManager::FileManager()
{

}

FileManager::~FileManager()
{

}

bool FileManager::LoadData(const std::wstring& path)
{
	ifstream iFile(L"..//" + path, ios::in);
	string buf = "";

	if (!iFile)
	{
		cout << "파일이 없습니다" << endl;
		DestroyWindow(Application.GetHwnd());
	}

	while (!iFile.eof())
	{
		getline(iFile, buf);
		cout << buf << endl;
	}

	iFile.close();

	// test용 
	mPlayerData = L"..//" + path;

	return S_OK;
}

bool FileManager::CreateSaveFile(const std::wstring& path, const std::wstring& name, eCharType type)
{
	if (mData[(UINT)type].size() >= 6)
		return false;

	ofstream fs(path + L"//" + name + L".csv", ios::app);

	// 기본 세팅
	fs << "Type," << to_string((UINT)type) << endl;
	fs << "Level," << to_string(1) << endl;
	fs << "Hp," << to_string(50) << endl;
	fs << "Mp," << to_string(50) << endl;
	fs << "Exp," << to_string(0) << endl;
	fs << "Name," << string(name.begin(), name.end()) << endl;
	fs << "X," << std::to_string(7500) << endl;
	fs << "Y," << std::to_string(7500) << endl;
	fs << "Z," << std::to_string(1) << endl;

	fs.close();

	ifstream ifs(path + L"//" + name + L".csv", ios::in);
	string tem = "";
	while (!ifs.eof())
	{
		getline(ifs, tem);
		wstring data(tem.begin(), tem.end());
		mCreateBuffer += data + L"\n";
	}

	mData[(UINT)type].push_back(mCreateBuffer);
	mDataPath[(UINT)type].push_back(mCreateBuffer);

	ifs.close();
	return true;
}

bool FileManager::ReadSaveFile(const std::wstring& path)
{
	ifstream iFile(L"..//" + path, ios::in);

	string buf = "";

	/*if (iFile == ifstream::)
	{
		cout << "파일이 없습니다" << endl;
		DestroyWindow(Application.GetHwnd());
	}*/

	while (!iFile.eof())
	{
		getline(iFile, buf);
	}
	iFile.close();

	return true;
}

bool FileManager::ReadSaveFiles(eFileType type)
{
	if (mDataPath[(UINT)type].empty())
		return false;

	for (wstring& str : mDataPath[(UINT)type])
	{
		ifstream iFile(str, ios::in);
		string buf = "";

	/*	if (!iFile)
		{
			cout << "파일이 없습니다" << endl;
			DestroyWindow(Application.GetHwnd());
		}*/

		wstring temp = L"";
		while (!iFile.eof())
		{
			getline(iFile, buf);
			wstring data(buf.begin(), buf.end());
			temp += data + L"\n";
		}

		mData[(UINT)type].push_back(temp);
		iFile.close();
	}

	return true;
}

bool FileManager::WriteSaveFile()
{
	GameObject* Object = WorldManager::GetInstance()->GetPlayer();
	Player* player = dynamic_cast<Player*>(Object);
	if (player == nullptr)
		return false;

	eCharType Type = player->GetCharType();
	int Level = player->GetLevel();
	float HP = player->GetHP();
	float Mp = player->GetMP();
	float Exp = player->GetExp();
	string strName = "Name," + player->GetSavename();
	Vector3 Pos = player->GetComponent<Transform>()->GetPosition();


	vector<string> data = {};
	{ // 쓰기 데이터 생성
		string strType = "Type," + to_string((UINT)Type);
		string strLevel = "Level," + to_string(Level);
		string strHP = "HP," + to_string(HP);
		string strMP = "MP," + to_string(Mp);
		string strExP = "EXP," + to_string(Exp);

		data.push_back(strType);
		data.push_back(strLevel);
		data.push_back(strHP);
		data.push_back(strMP);
		data.push_back(strExP);

		data.push_back(strName);

		string X = "X," + to_string(Pos.x);
		string Y = "Y," + to_string(Pos.y);
		string Z = "Z," + to_string(Pos.z);

		data.push_back(X);
		data.push_back(Y);
		data.push_back(Z);

		vector<ItemBase*> itemes = ItemManager::GetInstance()->GetSaveItemData();
		for (ItemBase* item : itemes)
		{
			UINT itemType = (UINT)item->GetItemType();
			UINT buttonStage = item->GetSlotInventory() == nullptr ? 0 : 1;
			Vector2 Index = item->GetIndex();

			string itemTypeStr = "ItemType," + to_string(itemType);
			string itembuttonStageStr = "ItemStage," + to_string(buttonStage);
			string ItemIndexX = "IndexX," + to_string(Index.x);
			string ItemIndexY = "IndexY," + to_string(Index.y);

			data.push_back(itemTypeStr);
			data.push_back(itembuttonStageStr);
			data.push_back(ItemIndexX);
			data.push_back(ItemIndexY);
		}
	}

	wstring str = mDataPath[(UINT)Type][player->GetSavePathIndex()];
	ofstream oFile(mDataPath[(UINT)Type][player->GetSavePathIndex()]);

	for (auto& str : data)
	{
		oFile << str << endl;
	}

	oFile.close();

	return true;
}

bool FileManager::ReadFold(const std::wstring& path, eFileType type)
{
	fs::path path_to_read(L"..//" + path);

	if (!fs::exists(path_to_read) || !fs::is_directory(path_to_read))
		cout << "폴더 열기 실패" << endl;

	vector<string> fileNames = {};

	for (const auto& file : fs::recursive_directory_iterator(path_to_read))
	{
		fileNames.push_back(file.path().filename().string());
	}

	for (string& str : fileNames)
	{
		wstring wstr(str.begin(), str.end());
		wstring Path = L"..//" + path + L"//" + wstr;

		mDataPath[(UINT)type].push_back(Path);
	}

	ReadSaveFiles(type);

	return true;
}
