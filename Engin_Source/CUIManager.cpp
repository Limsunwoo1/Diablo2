#include "CUIManager.h"
#include "CPanel.h"
#include "CButton.h"
#include "CHUD.h"
#include "CGameObject.h"

UIManager::UIManager()
{

}

UIManager::~UIManager()
{
	for (auto ui : mUIs)
	{
		if (ui.second == nullptr)
			continue;

		delete ui.second;
		ui.second = nullptr;
	}
	mUIs.clear();
}

void UIManager::Initialize()
{
	// 여기에서 ui 메모리에 할당하면 된다.

}

void UIManager::OnLoad(eUIType type)
{
	std::unordered_map<eUIType, UiBase*>::iterator iter = mUIs.find(type);
	if (iter == mUIs.end())
	{
		OnFail();
		return;
	}

	OnComplete(iter->second);
}

void UIManager::Tick()
{
	std::stack<UiBase*> uiBases = mUiBases;

	while (!uiBases.empty())
	{
		UiBase* uiBase = uiBases.top();
		if (uiBase != nullptr)
		{
			uiBase->Tick();
		}
		uiBases.pop();
	}

	std::map<UINT, pair<UiBase*, UiBase*>>::iterator iter;
	if (iter->second.first && iter->second.second)
	{
		iter->second.first->Tick();
		Vector2 pos = iter->second.first->GetPos();
		Vector2 size = iter->second.first->GetSize();

		iter->second.second->SetPos(Vector2(pos.x - (size.x * 0.5f), pos.y - (size.y * 0.5f)));
		iter->second.second->Tick();
	}
	if (mRequestUIQueue.size() > 0)
	{
		//UI 로드 해줘
		eUIType requestUI = mRequestUIQueue.front();
		mRequestUIQueue.pop();

		OnLoad(requestUI);
	}
}


void UIManager::Render(HDC hdc)
{
	std::stack<UiBase*> uiBases = mUiBases;
	std::stack<UiBase*> tempStack;

	// 뒤집어서랜더링을 해준다.
	while (!uiBases.empty())
	{
		UiBase* uiBase = uiBases.top();
		tempStack.push(uiBase);
		uiBases.pop();
	}

	while (!tempStack.empty())
	{
		UiBase* uiBase = tempStack.top();
		if (uiBase != nullptr)
		{
			uiBase->Render(hdc);
		}
		tempStack.pop();
	}
}

void UIManager::OnComplete(UiBase* addUI)
{
	if (addUI == nullptr)
		return;

	addUI->Initialize();
	addUI->Active();
	addUI->Tick();

	// addUI 가 전체화면 이면
	// Stack 안에 최상위 전체화면 Ui 를 꺼준다
	if (addUI->GetIsFullScreen())
	{
		std::stack<UiBase*>uiBases = mUiBases;
		while (!uiBases.empty())
		{
			UiBase* uiBase = uiBases.top();
			uiBases.pop();

			if (uiBase->GetIsFullScreen())
			{
				uiBase->InActive();
			}
		}
	}

	mUiBases.push(addUI);
}
void UIManager::OnFail()
{
	mCurrentData = nullptr;
}
void UIManager::Push(eUIType type, UiBase* ui)
{
	mUIs.insert(make_pair(type, ui));
}
void UIManager::Push(eUIType type)
{
	mRequestUIQueue.push(type);
}

void UIManager::Pop(eUIType type)
{
	if (mUiBases.size() <= 0)
		return;

	std::stack<UiBase*> tempStack;

	UiBase* uiBase = nullptr;
	while (mUiBases.size() > 0)
	{
		uiBase = mUiBases.top();
		mUiBases.pop();
		// pop하는 ui가 전체화면 일경우에,
		// 남은 ui중에 전체화면인 가장 상단의 ui 를 활성화 해주어야한다.
		if (uiBase->GetType() == type)
		{
			if (uiBase->GetIsFullScreen())
			{
				std::stack<UiBase*> uiBases = mUiBases;

				while (!uiBases.empty())
				{
					UiBase* tempUI = uiBases.top();
					uiBases.pop();

					if (tempUI->GetIsFullScreen())
					{
						tempUI->Active();
						break;
					}
				}
			}
			uiBase->InActive();
			uiBase->UIClear();
		}
		else
		{
			tempStack.push(uiBase);
		}

	}

	// UI 재정렬
	while (tempStack.size())
	{
		uiBase = tempStack.top();
		tempStack.pop();
		mUiBases.push(uiBase);
	}
}
inline void UIManager::DeleteUi(eUIType type)
{
	std::unordered_map<eUIType, UiBase*>::iterator iter;
	iter = mUIs.find(type);
	if (iter == mUIs.end())
		return;

	mUIs.erase(iter);
}

