#include "CSelectButtonSystem.h"
#include "FileManager.h"
#include "CResourceManager.h"

#include "CPlayerSelectButton.h"
#include "CSpriteRenderer.h"
#include "CMeshRenderer.h"


SelectButtonSystem::SelectButtonSystem()
	: UiBase(eUIType::Panel)
	, mClickButtonIdx(-1)
{
}

SelectButtonSystem::~SelectButtonSystem()
{
}

void SelectButtonSystem::Initalize()
{
	FileManager::GetInstance()->ReadFold(L"Resource//Data", eFileType::Char);

	int dataCnt = FileManager::GetInstance()->GetDataSize_Int(eFileType::Char);

	vector<wstring> temp = {};
	for (UINT i = 0; i < dataCnt; ++i)
	{
		temp.emplace_back(FileManager::GetInstance()->GetData(eFileType::Char,i));
	}

	for (wstring& str : temp)
	{
		
	}
}

void SelectButtonSystem::Update()
{
	
}

void SelectButtonSystem::FixedUpdate()
{
}

void SelectButtonSystem::Render()
{
}
