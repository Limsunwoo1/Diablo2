#pragma once
#include "CEngine.h"
#include "CUiBase.h"


/// <summary>
/// 플레이어의 조작에 영향을 받지 않죠 (위치 크기등등)
/// 전체 화면 UI
/// 기본 UI 
/// HUD 이미지와 정보만 잇는 UI
/// 버튼
/// 내 UI 내 자식구조로 여러가지 다른 ui들을 구성해서
/// 같이 움직이는 하나의 큰 UI 도 될수 있따.
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


