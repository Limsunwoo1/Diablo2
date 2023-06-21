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
	void Update();
	void FixedUpdate() {};
	void Render(HDC hdc);
	void OnComplete(UiBase* addUI);
	void OnFail();
	void OnLoad(eUIType type);

	void Push(const std::wstring& key, UiBase* ui);
	void Push(eUIType type);
	void Pop(eUIType type);

	void Rlease();

	template<typename T>
	inline T* GetUiInstance(const std::wstring& type)
	{
		return dynamic_cast<T*>(mUIs[type]);
	}

	inline void SetUiInstance(const std::wstring& key, UiBase* ui) { mUIs.insert(make_pair(key, ui)); }
	inline void DeleteUi(const std::wstring& type);
private:
	std::unordered_map<std::wstring, UiBase*> mUIs;
	std::queue<eUIType> mRequestUIQueue;
	std::stack<UiBase*> mUiBases;
	UiBase* mCurrentData;
};


