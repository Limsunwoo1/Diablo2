#include "CInventoryPanel.h"
#include "CObject.h"
#include "CEquipmentButton.h"
#include "CInventoryButton.h"
#include "CItemBase.h"

InventoryPanel::InventoryPanel()
	: Panel(eUIType::Panel)
	, mInvenArr(nullptr)
{
}

InventoryPanel::~InventoryPanel()
{
}

void InventoryPanel::Initalize()
{
	// Inventory
	{
		InventoryButton* button = new InventoryButton();
		button->SetName(L"InventoryButton");
		button->SetLayerType(eLayerType::UI);

		Transform* buttonTr = button->GetComponent<Transform>();
		buttonTr->SetPosition(Vector3(505.f, -130.f, 1.0f));
		buttonTr->SetSize(Vector3(540.f, 210.f, 1.0f));
		button->Initalize();

		button->OnActive();
		SetChild(button);
		button->SetParent(nullptr);
		
		mInvenArr = button;
	}

	// Shoes Item
	{
		EquipmentButton* button = new EquipmentButton(eEquipmentType::Shoes);
		button->SetName(L"ShoesButton");
		button->SetLayerType(eLayerType::UI);
		button->Initalize();


		Transform* buttonTr = button->GetComponent<Transform>();
		buttonTr->SetPosition(Vector3(720.f, 70.f, 0.5f));
		buttonTr->SetScale(Vector3(1.f, 1.0f, 0.0f));
		button->OnActive();

		SetChild(button);
		mSlotButton.emplace_back(button);
		button->SetParent(nullptr);
	}

	// Gloves Item
	{
		EquipmentButton* button = new EquipmentButton(eEquipmentType::Glove);
		button->SetLayerType(eLayerType::UI);
		button->Initalize();
		button->SetName(L"GloveButton");


		Transform* buttonTr = button->GetComponent<Transform>();
		buttonTr->SetPosition(Vector3(290.f, 70.f, 1.0f));
		buttonTr->SetScale(Vector3(1.f, 1.0f, 0.0f));
		button->OnActive();

		SetChild(button);
		mSlotButton.emplace_back(button);
		button->SetParent(nullptr);
	}

	// belt Item
	{
		EquipmentButton* button = new EquipmentButton(eEquipmentType::Belt);
		button->SetLayerType(eLayerType::UI);
		button->Initalize();
		button->SetName(L"BeltButton");


		Transform* buttonTr = button->GetComponent<Transform>();
		buttonTr->SetPosition(Vector3(505.f, 100.f, 1.0f));
		buttonTr->SetScale(Vector3(1.f, 0.5f, 0.0f));
		button->OnActive();

		SetChild(button);
		mSlotButton.emplace_back(button);
		button->SetParent(nullptr);
	}

	// weapon1 Item
	{
		EquipmentButton* button = new EquipmentButton(eEquipmentType::Wepon);
		button->SetLayerType(eLayerType::UI);
		button->Initalize();
		button->SetName(L"WeponButton");


		Transform* buttonTr = button->GetComponent<Transform>();
		buttonTr->SetPosition(Vector3(290.f, 260.f, 1.0f));
		buttonTr->SetScale(Vector3(1.f, 2.0f, 0.0f));
		button->OnActive();

		SetChild(button);
		mSlotButton.emplace_back(button);
		button->SetParent(nullptr);
	}

	// weapon2 Item
	{
		EquipmentButton* button = new EquipmentButton(eEquipmentType::Wepon);
		button->SetLayerType(eLayerType::UI);
		button->Initalize();
		button->SetName(L"WeponButton2");


		Transform* buttonTr = button->GetComponent<Transform>();
		buttonTr->SetPosition(Vector3(720.f, 260.f, 1.0f));
		buttonTr->SetScale(Vector3(1.f, 2.0f, 0.0f));
		button->OnActive();

		SetChild(button);
		mSlotButton.emplace_back(button);
		button->SetParent(nullptr);
	}

	// Suit Item
	{
		EquipmentButton* button = new EquipmentButton(eEquipmentType::Suit);
		button->SetLayerType(eLayerType::UI);
		button->Initalize();
		button->SetName(L"SuitButton");


		Transform* buttonTr = button->GetComponent<Transform>();
		buttonTr->SetPosition(Vector3(505.f, 235.f, 1.0f));
		buttonTr->SetScale(Vector3(1.f, 1.5f, 0.0f));
		button->OnActive();

		SetChild(button);
		mSlotButton.emplace_back(button);
		button->SetParent(nullptr);
	}

	// Cap Item
	{
		EquipmentButton* button = new  EquipmentButton(eEquipmentType::Cap);
		button->SetLayerType(eLayerType::UI);
		button->Initalize();
		button->SetName(L"CapButton");


		Transform* buttonTr = button->GetComponent<Transform>();
		buttonTr->SetPosition(Vector3(505.f, 390.f, 1.0f));
		buttonTr->SetScale(Vector3(1.f, 1.0f, 0.0f));
		button->OnActive();

		SetChild(button);
		mSlotButton.emplace_back(button);
		button->SetParent(nullptr);
	}
}

void InventoryPanel::Update()
{
	Panel::Update();
}

void InventoryPanel::FixedUpdate()
{
	Panel::FixedUpdate();
}

void InventoryPanel::Render()
{
	Panel::Render();
}

void InventoryPanel::InitAnimation()
{
}

void InventoryPanel::OnActive()
{
	mbEnable = true;

	for (UiBase* child : mChilds)
	{
		if (child == nullptr)
			continue;

		child->Active();
	}
}

void InventoryPanel::UnActive()
{
	mbEnable = false;

	for (UiBase* child : mChilds)
	{
		if (child == nullptr)
			continue;

		child->UnActive();
	}
}
