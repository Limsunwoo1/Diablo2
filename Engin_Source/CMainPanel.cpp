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
		tr->SetScale(Vector3(ITEM_X_SIZE, ITEM_Y_SIZE, 1.0f));
		tr->SetPosition(Vector3(1.83f, -3.0, 1.0f));


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
		tr->SetScale(Vector3(ITEM_X_SIZE, ITEM_Y_SIZE, 1.0f));
		tr->SetPosition(Vector3(2.50f, -3.0, 1.0f));


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
		tr->SetScale(Vector3(ITEM_X_SIZE, ITEM_Y_SIZE, 1.0f));
		tr->SetPosition(Vector3(3.15f, -3.0, 1.0f));


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
		tr->SetScale(Vector3(ITEM_X_SIZE, ITEM_Y_SIZE, 1.0f));
		tr->SetPosition(Vector3(3.8f, -3.0, 1.0f));


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

			if (potion)
			{
				potion->UsePotion();
				mPotionSlot[0]->SetItem(nullptr);
				mPotionSlot[0]->SetUsed(false);
			}
		}
	}
	else if (Input::GetInstance()->GetKeyDown(eKeyCode::N_2))
	{
		ItemBase* item = mPotionSlot[1]->GetItem();

		if (item != nullptr)
		{
			PotionItem* potion = dynamic_cast<PotionItem*>(item);

			if (potion)
			{
				potion->UsePotion();
				mPotionSlot[1]->SetItem(nullptr);
				mPotionSlot[1]->SetUsed(false);
			}
		}
	}
	else if (Input::GetInstance()->GetKeyDown(eKeyCode::N_3))
	{
		ItemBase* item = mPotionSlot[2]->GetItem();

		// test
		for (int i = 0; i < 4; ++i)
		{
			if (i == 2)
				continue;

			if (mPotionSlot[i]->GetItem() == item)
				int a = 0;
		}
		if (item != nullptr)
		{
			PotionItem* potion = dynamic_cast<PotionItem*>(item);

			if (potion)
			{
				potion->UsePotion();
				mPotionSlot[2]->SetItem(nullptr);
				mPotionSlot[2]->SetUsed(false);
			}
		}
	}
	else if (Input::GetInstance()->GetKeyDown(eKeyCode::N_4))
	{
		ItemBase* item = mPotionSlot[3]->GetItem();

		if (item != nullptr)
		{
			PotionItem* potion = dynamic_cast<PotionItem*>(item);

			if (potion)
			{
				potion->UsePotion();
				mPotionSlot[3]->SetItem(nullptr);
				mPotionSlot[3]->SetUsed(false);
			}
		}
	}
}
