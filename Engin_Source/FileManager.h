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

	bool CreateSaveFile(const std::wstring& path, const std::wstring& name, eCharType type);
	bool ReadSaveFile(const std::wstring& path);
	bool ReadSaveFiles(eFileType type);
	bool WriteSaveFile();

	bool ReadFold(const std::wstring& path, eFileType type);

	const wstring& GetData(eFileType type, UINT index) { return mData[(UINT)type][index];}
	UINT GetDataSize_Int(eFileType type) { return mData[(UINT)type].size(); }

	void SetPlayerData(const wstring& data) { mPlayerData = data; }
private:
	vector<wstring> mDataPath[(UINT)eFileType::End];
	vector<wstring> mData[(UINT)eFileType::End];

	wstring mPlayerData;
};

