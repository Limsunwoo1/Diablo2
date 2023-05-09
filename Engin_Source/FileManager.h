#pragma once
#include <filesystem>
#include <fstream>
#include "CEngine.h"


using namespace std;


class FileManager
{
	SINGLE(FileManager)
public:
	HRESULT LoadData(const std::wstring& path);

	HRESULT CreateSaveFile(const std::wstring& path);
	HRESULT ReadSaveFile(const std::wstring& path);
	HRESULT WriteSaveFile(const std::wstring& path);

	vector<string> ReadFold(const std::wstring& path);

private:
	vector<string> mCharSaveData;
};

