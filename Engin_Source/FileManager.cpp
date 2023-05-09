#include "FileManager.h"
#include "CWorldManager.h"
#include "CApplication.h"
#include "Cplayer.h"

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
		cout << "������ �����ϴ�" << endl;
		DestroyWindow(Application.GetHwnd());
	}

	while (!iFile.eof())
	{
		getline(iFile, buf);
		cout << buf << endl;
	}

	iFile.close();

	// test�� 
	mPlayerData = L"..//" + path;

	return S_OK;
}

bool FileManager::CreateSaveFile(const std::wstring& path)
{
	return S_OK;
}

bool FileManager::ReadSaveFile(const std::wstring& path)
{
	ifstream iFile(L"..//" + path, ios::in);

	string buf = "";

	/*if (iFile == ifstream::)
	{
		cout << "������ �����ϴ�" << endl;
		DestroyWindow(Application.GetHwnd());
	}*/

	while (!iFile.eof())
	{
		getline(iFile, buf);
		cout << buf << endl;
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
			cout << "������ �����ϴ�" << endl;
			DestroyWindow(Application.GetHwnd());
		}*/

		wstring temp = L"";
		while (!iFile.eof())
		{
			getline(iFile, buf);
			wstring data(buf.begin(), buf.end());
			temp += data + L"\n";
			cout << buf << endl;
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

	vector<string> data = {};
	{ // ���� ������ ����
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
	}

	ofstream oFile(mPlayerData);

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
		cout << "���� ���� ����" << endl;

	vector<string> fileNames = {};

	for (const auto& file : fs::directory_iterator(path_to_read))
	{
		fileNames.push_back(file.path().filename().string());
	}

	for (string& str : fileNames)
	{
		wstring wstr(str.begin(), str.end());
		wstring Path = L"..//" + path + L"//" + wstr;

		mDataPath[(UINT)type].push_back(Path);
	}

	if (ReadSaveFiles(type))
		mDataPath->clear();

	return true;
}
