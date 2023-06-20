#include "CMainPanel.h"
#include "CInput.h"
#include "CInventoryButton.h"
#include "CPotionItem.h"

MainPanel::MainPanel()
	: Panel(eUIType::Panel)
	, mPotionSlot{}
{
	InitPotionSlot();
}

MainPanel::~MainPanel()
{
}

void MainPanel::Update()
{
	UsePotion();
	RenderOn();
	Panel::Update();
}

void MainPanel::FixedUpdate()
{
	Panel::FixedUpdate();
}

void MainPanel::InitPotionSlot()
{
	{
		EquipmentButton* button = new EquipmentButton(eEquipmentType::Potion);
		button->Initalize();
		button->SetLayerType(eLayerType::UI);
		button->OnActive();

		button->SetName(L"PotionButton1");

		Transform* tr = button->GetComponent<Transform>();
		tr->SetSize(Vector3(ITEM_X_SIZE, ITEM_Y_SIZE, 1.0f));
		tr->SetPosition(Vector3(80.f, -400.f, 1.0f));


		this->SetChild(button);
		button->SetParent(nullptr);

		mPotionSlot.emplace_back(button);
	}

	{
		EquipmentButton* button = new EquipmentButton(eEquipmentType::Potion);
		button->Initalize();
		button->SetLayerType(eLayerType::UI);
		button->OnActive();

		button->SetName(L"PotionButton2");

		Transform* tr = button->GetComponent<Transform>();
		tr->SetSize(Vector3(ITEM_X_SIZE, ITEM_Y_SIZE, 1.0f));
		tr->SetPosition(Vector3(150.f, -400.f, 1.0f));


		this->SetChild(button);
		button->SetParent(nullptr);

		mPotionSlot.emplace_back(button);
	}

	{
		EquipmentButton* button = new EquipmentButton(eEquipmentType::Potion);
		button->Initalize();
		button->SetLayerType(eLayerType::UI);
		button->OnActive();

		button->SetName(L"PotionButton3");

		Transform* tr = button->GetComponent<Transform>();
		tr->SetSize(Vector3(ITEM_X_SIZE, ITEM_Y_SIZE, 1.0f));
		tr->SetPosition(Vector3(215., -400.f, 1.0f));


		this->SetChild(button);
		button->SetParent(nullptr);

		mPotionSlot.emplace_back(button);
	}

	{
		EquipmentButton* button = new EquipmentButton(eEquipmentType::Potion);
		button->Initalize();
		button->SetLayerType(eLayerType::UI);
		button->OnActive();

		button->SetName(L"PotionButton4");

		Transform* tr = button->GetComponent<Transform>();
		tr->SetSize(Vector3(ITEM_X_SIZE, ITEM_Y_SIZE, 1.0f));
		tr->SetPosition(Vector3(280.f, -400.f, 1.0f));


		this->SetChild(button);
		button->SetParent(nullptr);

		mPotionSlot.emplace_back(button);
	}
}

void MainPanel::UsePotion()
{
	if (Input::GetInstance()->GetKeyDown(eKeyCode::N_1))
	{
		ItemBase* item = mPotionSlot[0]->GetItem();

		if (item != nullptr)
		{
			PotionItem* potion = dynamic_cast<PotionItem*>(item);

			if (potion && potion->GetState() == eState::active)
			{
				mPotionSlot[0]->SetItem(nullptr);
				mPotionSlot[0]->SetUsed(false);
				potion->UsePotion();
			}
		}
	}
	else if (Input::GetInstance()->GetKeyDown(eKeyCode::N_2))
	{
		ItemBase* item = mPotionSlot[1]->GetItem();

		if (item != nullptr)
		{
			PotionItem* potion = dynamic_cast<PotionItem*>(item);

			if (potion && potion->GetState() == eState::active)
			{
				mPotionSlot[1]->SetItem(nullptr);
				mPotionSlot[1]->SetUsed(false);
				potion->UsePotion();
			}
		}
	}
	else if (Input::GetInstance()->GetKeyDown(eKeyCode::N_3))
	{
		ItemBase* item = mPotionSlot[2]->GetItem();

		if (item != nullptr)
		{
			PotionItem* potion = dynamic_cast<PotionItem*>(item);

			if (potion && potion->GetState() == eState::active)
			{
				mPotionSlot[2]->SetItem(nullptr);
				mPotionSlot[2]->SetUsed(false);
				potion->UsePotion();
			}
		}
	}
	else if (Input::GetInstance()->GetKeyDown(eKeyCode::N_4))
	{
		ItemBase* item = mPotionSlot[3]->GetItem();

		if (item != nullptr)
		{
			PotionItem* potion = dynamic_cast<PotionItem*>(item);

			if (potion && potion->GetState() == eState::active)
			{
				mPotionSlot[3]->SetItem(nullptr);
				mPotionSlot[3]->SetUsed(false);
				potion->UsePotion();
			}
		}
	}
}

void MainPanel::RenderOn()
{
	for (EquipmentButton* slot : mPotionSlot)
	{
		if (slot == nullptr)
			continue;

		if (slot->GetItem() == nullptr)
			continue;

		slot->GetItem()->GetComponent<BaseRenderer>()->SetRenderStop(false);
	}
}
