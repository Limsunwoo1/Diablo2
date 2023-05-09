#include "FileManager.h"
#include <iostream>

#include "CApplication.h"

extern CApplication Application;


namespace fs = std::filesystem;

FileManager::FileManager()
{

}

FileManager::~FileManager()
{

}

HRESULT FileManager::LoadData(const std::wstring& path)
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

	return S_OK;
}

HRESULT FileManager::CreateSaveFile(const std::wstring& path)
{
	return E_NOTIMPL;
}

HRESULT FileManager::ReadSaveFile(const std::wstring& path)
{
	return E_NOTIMPL;
}

HRESULT FileManager::WriteSaveFile(const std::wstring& path)
{
	return E_NOTIMPL;
}

vector<string> FileManager::ReadFold(const std::wstring& path)
{
	fs::path path_to_read(L"..//" + path);

	if (!fs::exists(path_to_read) || !fs::is_directory(path_to_read))
		cout << "폴더 열기 실패" << endl;

	vector<string> fileNames = {};

	for (const auto& file : fs::directory_iterator(path_to_read))
	{
		fileNames.push_back(file.path().filename().string());
	}

	for (const string& str : fileNames)
	{
		wstring wstr(str.begin(), str.end());
		ReadSaveFile(wstr);
	}

	return fileNames;
}
