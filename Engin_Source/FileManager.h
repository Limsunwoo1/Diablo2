#pragma once
#include <filesystem>
#include <fstream>
#include "CEngine.h"


using namespace std;


class FileManager
{
	SINGLE(FileManager)
public:
	bool LoadData(const std::wstring& path);

	bool CreateSaveFile(const std::wstring& path);

	bool ReadSaveFile(const std::wstring& path);
	bool ReadSaveFile(eFileType type);

	bool WriteSaveFile();

	bool ReadFold(const std::wstring& path, eFileType type);

private:
	vector<wstring> mDataPath[(UINT)eFileType::End];
	wstring mPlayerData;
};

