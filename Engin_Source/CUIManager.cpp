#include "CUIManager.h"
#include "CPanel.h"
#include "CButton.h"
#include "CHUD.h"
#include "CGameObject.h"
#include "CObject.h"
#include "CCollider2D.h"
#include "CInventoryPanel.h"
#include "CMainPanel.h"
#include "CEquipmentButton.h"

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
		InventoryPanel* Inventory = new InventoryPanel();
		Inventory->SetName(L"InventoryPanel");
		Object::Instantiate<InventoryPanel>(eLayerType::UI, eSceneType::Play, Inventory);
		Inventory->InitRenderer(L"InventoryMaterial", L"InventoryPanel", L"UI//invenpanel.png");
		Inventory->UnActive();
		Transform* InventoryTR = Inventory->GetComponent<Transform>();
		InventoryTR->SetPosition(Vector3(500.f, 50.f, 0.01f));
		InventoryTR->SetSize(Vector3(600.f, 800.f, 1.0f));

		/*Collider2D* InventoryCollider = Inventory->AddComponent<Collider2D>();
		InventoryCollider->SetType(eColliderType::Rect);
		InventoryCollider->SetOwner(Inventory);
		InventoryCollider->Initalize();*/
		Push(L"mainInventory", Inventory);

		///////////////////////////////////////////////////////////////////////////////
		MainPanel* mainPanelui = new MainPanel();
		Object::Instantiate<Panel>(eLayerType::UI, eSceneType::Play, mainPanelui);
		mainPanelui->InitRenderer(L"mainPanelMaterial", L"mainPanel", L"UI//mainPanel.png");
		mainPanelui->Active();
		Transform* maintr = mainPanelui->GetComponent<Transform>();
		maintr->SetPosition(Vector3(0.f, -400.f, 0.01));
		maintr->SetSize(Vector3(1000.f, 100.f, 0.f));
		Push(L"mainPanel", mainPanelui);
		///////////////////////////////////////////////////////////////////////////////

		/*Collider2D* InventoryCollider = Inventory->AddComponent<Collider2D>();
		InventoryCollider->SetType(eColliderType::Rect);
		InventoryCollider->SetOwner(Inventory);
		InventoryCollider->Initalize();*/
		Push(L"mainInventory", Inventory);

		///////////////////////////////////////////////////////////////////////////////
		Panel* hpui = new Panel(eUIType::Panel);
		hpui->SetName(L"HpPanel");
		Object::Instantiate<Panel>(eLayerType::UI, eSceneType::Play, hpui);
		hpui->InitRenderer(L"HpPanelMaterial", L"HPPanel", L"UI//ctrlpanellife.png");
		hpui->Active();
		Transform* hpuitr = hpui->GetComponent<Transform>();
		hpuitr->SetPosition(Vector3(-650.f, 50.f, 0.02f));
		hpuitr->SetSize(Vector3(300.f, 300.f, 1.0f));
		//Push(L"hpUi", hpui);

		///////////////////////////////////////////////////////////////////////////////
		Panel* mpui = new Panel(eUIType::Panel);
		mpui->SetName(L"MpPanel");
		Object::Instantiate<Panel>(eLayerType::UI, eSceneType::Play, mpui);
		mpui->InitRenderer(L"MpPanelMaterial", L"MPPanel", L"UI//ctrlpanelmana.png");
		mpui->Active();
		Transform* mpuitr = mpui->GetComponent<Transform>();
		mpuitr->SetPosition(Vector3(650.f, 50.f, 0.02f));
		mpuitr->SetSize(Vector3(300.f, 300.f, 1.f));
		//Push(L"mpUi", mpui);



		// ui childs

		// HP MP
		Panel* hp = new Panel(eUIType::Panel);
		hp->SetName(L"Hp");
		hp->InitRenderer(L"HPMaterial", L"HP", L"UI//life.png");
		hp->Active();

		Transform* hptr = hp->GetComponent<Transform>();
		hptr->SetPosition(Vector3(30.f, 0.f, 0.02f));
		hptr->SetSize(Vector3(220.0f, 220.0f, 0.f));
		hptr->SetRotation(Vector3(0.0f, 0.0f, 90.f));


		Panel* mp = new Panel(eUIType::Panel);
		mp->SetName(L"Mp");
		mp->InitRenderer(L"MPMaterial", L"MP", L"UI//mana.png");
		mp->Active();

		Transform* mptr = mp->GetComponent<Transform>();
		mptr->SetPosition(Vector3(-30.f, 0.f, 0.02f));
		mptr->SetSize(Vector3(220.0f, 220.0f, 0.f));
		mptr->SetRotation(Vector3(0.0f, 0.0f, 90.f));

		// RunIcon
		Panel* runIcon = new Panel(eUIType::Panel);
		runIcon->SetName(L"RunIcon");
		runIcon->InitRenderer(L"RunIconMaterial", L"RunIcon", L"UI//RunIcon.png");
		runIcon->Active();

		Transform* runtr = runIcon->GetComponent<Transform>();
		runtr->SetPosition(Vector3(-291.f, -10.f, 0.02f));
		runtr->SetSize(Vector3(35.f, 35.f, 1.f));

		// WalkIcon
		Panel* walkIcon = new Panel(eUIType::Panel);
		walkIcon->SetName(L"WalkIcon");
		walkIcon ->InitRenderer(L"WalkIconMaterial", L"WalkIcon", L"UI//WalkIcon.png");
		walkIcon ->Active();

		Transform* walktr = walkIcon->GetComponent<Transform>();
		walktr->SetPosition(Vector3(-291.f, -10.f, 0.02f));
		walktr->SetSize(Vector3(35.f, 35.f, 1.f));

		// RunGauge
		Panel* runGauge = new Panel(eUIType::Panel);
		runGauge->SetName(L"RunGauge");
		runGauge->InitRenderer(L"RunGaugeMaterial", L"RunGauge", L"UI//RunGauge.png");
		runGauge->Active();

		Transform* RunGaugetr = runGauge->GetComponent<Transform>();
		RunGaugetr->SetPosition(Vector3(-157.f, -11.f, 0.02f));
		RunGaugetr->SetSize(Vector3(2.2f * 100.f, 0.35f * 100.f, 0.f));

		
		// 부모자식 연결
		mainPanelui->SetChild(hpui);
		mainPanelui->SetChild(mpui);
		mainPanelui->SetChild(runIcon);
		mainPanelui->SetChild(walkIcon);
		mainPanelui->SetChild(runGauge);

		hpui->SetChild(hp);
		mpui->SetChild(mp);
	}
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
	//std::stack<UiBase*> uiBases = mUiBases;
	//std::stack<UiBase*> tempStack;

	//// 뒤집어서랜더링을 해준다.
	//while (!uiBases.empty())
	//{
	//	UiBase* uiBase = uiBases.top();
	//	tempStack.push(uiBase);
	//	uiBases.pop();
	//}

	//while (!tempStack.empty())
	//{
	//	UiBase* uiBase = tempStack.top();
	//	if (uiBase != nullptr)
	//	{
	//		uiBase->Render(hdc);
	//	}
	//	tempStack.pop();
	//}
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
void UIManager::Push(const std::wstring& key, UiBase* ui)
{
	mUIs.insert(make_pair(key, ui));
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
inline void UIManager::DeleteUi(const std::wstring& type)
{
	std::unordered_map<std::wstring, UiBase*>::iterator iter;
	iter = mUIs.find(type);
	if (iter == mUIs.end())
		return;

	mUIs.erase(iter);
}