#pragma once
#include "CEngine.h"
#include "CUiBase.h"


/// <summary>
/// �÷��̾��� ���ۿ� ������ ���� ���� (��ġ ũ����)
/// ��ü ȭ�� UI
/// �⺻ UI 
/// HUD �̹����� ������ �մ� UI
/// ��ư
/// �� UI �� �ڽı����� �������� �ٸ� ui���� �����ؼ�
/// ���� �����̴� �ϳ��� ū UI �� �ɼ� �ֵ�.
/// </summary>

using namespace std;
using namespace Math;
using namespace graphics;


class UIManager
{
	SINGLE(UIManager);

public:
	void Initialize();
	void Tick();
	void Render(HDC hdc);
	void OnComplete(UiBase* addUI);
	void OnFail();
	void OnLoad(eUIType type);

	void Push(eUIType type, UiBase* ui);
	void Push(eUIType type);
	void Pop(eUIType type);

	template<typename T>
	inline T* GetUiInstance(eUIType type)
	{
		return dynamic_cast<T*>(mUIs[type]);
	}

	inline void SetUiInstance(eUIType type, UiBase* ui) { mUIs.insert(make_pair(type, ui)); }
	inline void DeleteUi(eUIType type);
	inline void DeleteMonsterHp(UINT key);
private:
	std::unordered_map<eUIType, UiBase*> mUIs;
	std::queue<eUIType> mRequestUIQueue;
	std::stack<UiBase*> mUiBases;
	UiBase* mCurrentData;
};


