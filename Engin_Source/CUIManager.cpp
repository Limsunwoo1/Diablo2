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
	// ���⿡�� ui �޸𸮿� �Ҵ��ϸ� �ȴ�.

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
		//UI �ε� ����
		eUIType requestUI = mRequestUIQueue.front();
		mRequestUIQueue.pop();

		OnLoad(requestUI);
	}
}


void UIManager::Render(HDC hdc)
{
	std::stack<UiBase*> uiBases = mUiBases;
	std::stack<UiBase*> tempStack;

	// ������������� ���ش�.
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

	// addUI �� ��üȭ�� �̸�
	// Stack �ȿ� �ֻ��� ��üȭ�� Ui �� ���ش�
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
		// pop�ϴ� ui�� ��üȭ�� �ϰ�쿡,
		// ���� ui�߿� ��üȭ���� ���� ����� ui �� Ȱ��ȭ ���־���Ѵ�.
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

	// UI ������
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

