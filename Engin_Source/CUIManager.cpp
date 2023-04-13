#include "CUIManager.h"
#include "CPanel.h"
#include "CButton.h"
#include "CHUD.h"
#include "CGameObject.h"
#include "CObject.h"

UIManager::UIManager()
	: mCurrentData(nullptr)
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

	//mainPanelMaterial

	{
		Panel* mainPanelui = new Panel(eUIType::Panel);
		Object::Instantiate<Panel>(eLayerType::UI, eSceneType::Play, mainPanelui);
		mainPanelui->InitRenderer(L"mainPanelMaterial", L"mainPanel", L"UI//mainPanel.png");
		mainPanelui->Active();
		Transform* maintr = mainPanelui->GetComponent<Transform>();
		maintr->SetPosition(Vector3(1.f, -3.f, 1.0f));
		maintr->SetScale(Vector3(10.f, 1.f, 0.f));
		Push(eUIType::Panel, mainPanelui);

		Panel* hpui = new Panel(eUIType::Panel);
		Object::Instantiate<Panel>(eLayerType::UI, eSceneType::Play, hpui);
		hpui->InitRenderer(L"HpPanelMaterial", L"HPPanel", L"UI//ctrlpanellife.png");
		hpui->Active();
		Transform* hpuitr = hpui->GetComponent<Transform>();
		hpuitr->SetPosition(Vector3(-6.5f, 0.5f, 1.0f));
		hpuitr->SetScale(Vector3(3.f, 3.f, 0.f));
		Push(eUIType::Panel, hpui);

		Panel* mpui = new Panel(eUIType::Panel);
		Object::Instantiate<Panel>(eLayerType::UI, eSceneType::Play, mpui);
		mpui->InitRenderer(L"MpPanelMaterial", L"MPPanel", L"UI//ctrlpanelmana.png");
		mpui->Active();
		Transform* mpuitr = mpui->GetComponent<Transform>();
		mpuitr->SetPosition(Vector3(6.5f, 0.5f, 1.0f));
		mpuitr->SetScale(Vector3(3.f, 3.f, 0.f));
		Push(eUIType::Panel, mpui);


		// ui childs

		// HP MP
		Panel* hp = new Panel(eUIType::Panel);
		hp->InitRenderer(L"HPMaterial", L"HP", L"UI//life.png");
		hp->Active();

		Transform* hptr = hp->GetComponent<Transform>();
		hptr->SetPosition(Vector3(0.2f, 0.15f, 0.f));
		hptr->SetScale(Vector3(2.0f, 2.0f, 0.f));
		hptr->SetRotation(Vector3(0.0f, 0.0f, 90.f));


		Panel* mp = new Panel(eUIType::Panel);
		mp->InitRenderer(L"MPMaterial", L"MP", L"UI//mana.png");
		mp->Active();

		Transform* mptr = mp->GetComponent<Transform>();
		mptr->SetPosition(Vector3(-0.2f, 0.15f, 0.f));
		mptr->SetScale(Vector3(2.0f, 2.0f, 0.f));
		mptr->SetRotation(Vector3(0.0f, 0.0f, 90.f));

		// RunIcon
		Panel* runIcon = new Panel(eUIType::Panel);
		runIcon->InitRenderer(L"RunIconMaterial", L"RunIcon", L"UI//RunIcon.png");
		runIcon->Active();

		Transform* runtr = runIcon->GetComponent<Transform>();
		runtr->SetPosition(Vector3(-2.9f, -0.12f, 0.f));
		runtr->SetScale(Vector3(0.3f, 0.3f, 0.f));

		// WalkIcon
		Panel* walkIcon = new Panel(eUIType::Panel);
		walkIcon ->InitRenderer(L"WalkIconMaterial", L"WalkIcon", L"UI//WalkIcon.png");
		walkIcon ->Active();

		Transform* walktr = walkIcon->GetComponent<Transform>();
		walktr->SetPosition(Vector3(-2.9f, -0.12f, 0.f));
		walktr->SetScale(Vector3(0.3f, 0.3f, 0.f));

		// RunGauge
		Panel* runGauge = new Panel(eUIType::Panel);
		runGauge->InitRenderer(L"RunGaugeMaterial", L"RunGauge", L"UI//RunGauge.png");
		runGauge->Active();

		Transform* RunGaugetr = runGauge->GetComponent<Transform>();
		RunGaugetr->SetPosition(Vector3(-1.59f, -0.12f, 0.f));
		RunGaugetr->SetScale(Vector3(2.2f, 0.35f, 0.f));

		
		// 부모자식 연결
		mainPanelui->SetChild(hpui);
		mainPanelui->SetChild(mpui);
		mainPanelui->SetChild(runIcon);
		mainPanelui->SetChild(walkIcon);
		mainPanelui->SetChild(runGauge);

		hpui->SetChild(hp);
		mpui->SetChild(mp);
	}

	/*{
		UiBase* ui = new UiBase(eUIType::HUD);
		Push(eUIType::HUD, ui);
	}

	{
		UiBase* ui = new UiBase(eUIType::HUD);
		Push(eUIType::HUD, ui);
	}

	{
		UiBase* ui = new UiBase(eUIType::HUD);
		Push(eUIType::HUD, ui);
	}*/
}

void UIManager::OnLoad(eUIType type)
{
	/*std::unordered_map<eUIType, UiBase*>::iterator iter = mUIs.find(type);
	if (iter == mUIs.end())
	{
		OnFail();
		return;
	}

	OnComplete(iter->second);*/
}

void UIManager::Update()
{
	//std::stack<UiBase*> uiBases = mUiBases;

	//while (!uiBases.empty())
	//{
	//	UiBase* uiBase = uiBases.top();
	//	if (uiBase != nullptr)
	//	{
	//		uiBase->Update();
	//	}
	//	uiBases.pop();
	//}

	//std::map<UINT, pair<UiBase*, UiBase*>>::iterator iter;
	//if (iter->second.first && iter->second.second)
	//{
	//	iter->second.first->Update();
	//	//Vector2 pos = iter->second.first->GetPos();
	//	//Vector2 size = iter->second.first->GetSize();

	//	//iter->second.second->SetPos(Vector2(pos.x - (size.x * 0.5f), pos.y - (size.y * 0.5f)));
	//	iter->second.second->Update();
	//}
	//if (mRequestUIQueue.size() > 0)
	//{
	//	//UI 로드 해줘
	//	eUIType requestUI = mRequestUIQueue.front();
	//	mRequestUIQueue.pop();

	//	OnLoad(requestUI);
	//}
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
	//if (addUI == nullptr)
	//	return;

	//addUI->Initialize();
	//addUI->Active();
	//addUI->Update();

	//// addUI 가 전체화면 이면
	//// Stack 안에 최상위 전체화면 Ui 를 꺼준다
	//if (addUI->GetIsFullScreen())
	//{
	//	std::stack<UiBase*>uiBases = mUiBases;
	//	while (!uiBases.empty())
	//	{
	//		UiBase* uiBase = uiBases.top();
	//		uiBases.pop();

	//		if (uiBase->GetIsFullScreen())
	//		{
	//			uiBase->InActive();
	//		}
	//	}
	//}

	//mUiBases.push(addUI);
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