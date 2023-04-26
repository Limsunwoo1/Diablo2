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
		button->SetLayerType(eLayerType::UI);

		Transform* buttonTr = button->GetComponent<Transform>();
		buttonTr->SetPosition(Vector3(6.05f, -0.3f, 1.0f));
		buttonTr->SetScale(Vector3(5.4f, 2.1f, 0.0f));
		button->Initalize();

		button->OnActive();
		SetChild(button);
		button->SetParent(nullptr);
		
		mInvenArr = button;
	}

	// Shoes Item
	{
		EquipmentButton* button = new EquipmentButton(eEquipmentType::Shoes);
		button->SetLayerType(eLayerType::UI);
		button->Initalize();


		Transform* buttonTr = button->GetComponent<Transform>();
		buttonTr->SetPosition(Vector3(8.2f, 1.7f, 0.5f));
		buttonTr->SetScale(Vector3(1.f, 1.0f, 0.0f));
		button->OnActive();

		SetChild(button);
		button->SetParent(nullptr);
	}

	// Gloves Item
	{
		EquipmentButton* button = new EquipmentButton(eEquipmentType::Glove);
		button->SetLayerType(eLayerType::UI);
		button->Initalize();


		Transform* buttonTr = button->GetComponent<Transform>();
		buttonTr->SetPosition(Vector3(3.9f, 1.7f, 1.0f));
		buttonTr->SetScale(Vector3(1.f, 1.0f, 0.0f));
		button->OnActive();

		SetChild(button);
		button->SetParent(nullptr);
	}

	// belt Item
	{
		EquipmentButton* button = new EquipmentButton(eEquipmentType::Belt);
		button->SetLayerType(eLayerType::UI);
		button->Initalize();


		Transform* buttonTr = button->GetComponent<Transform>();
		buttonTr->SetPosition(Vector3(6.05f, 2.0f, 1.0f));
		buttonTr->SetScale(Vector3(1.f, 0.5f, 0.0f));
		button->OnActive();

		SetChild(button);
		button->SetParent(nullptr);
	}

	// weapon1 Item
	{
		EquipmentButton* button = new EquipmentButton(eEquipmentType::Wepon);
		button->SetLayerType(eLayerType::UI);
		button->Initalize();


		Transform* buttonTr = button->GetComponent<Transform>();
		buttonTr->SetPosition(Vector3(3.9f, 3.6f, 1.0f));
		buttonTr->SetScale(Vector3(1.f, 2.0f, 0.0f));
		button->OnActive();

		SetChild(button);
		button->SetParent(nullptr);
	}

	// weapon2 Item
	{
		EquipmentButton* button = new EquipmentButton(eEquipmentType::Wepon);
		button->SetLayerType(eLayerType::UI);
		button->Initalize();


		Transform* buttonTr = button->GetComponent<Transform>();
		buttonTr->SetPosition(Vector3(8.2f, 3.6f, 1.0f));
		buttonTr->SetScale(Vector3(1.f, 2.0f, 0.0f));
		button->OnActive();

		SetChild(button);
		button->SetParent(nullptr);
	}

	// Suit Item
	{
		EquipmentButton* button = new EquipmentButton(eEquipmentType::Suit);
		button->SetLayerType(eLayerType::UI);
		button->Initalize();


		Transform* buttonTr = button->GetComponent<Transform>();
		buttonTr->SetPosition(Vector3(6.05f, 3.3f, 1.0f));
		buttonTr->SetScale(Vector3(1.f, 1.5f, 0.0f));
		button->OnActive();

		SetChild(button);
		button->SetParent(nullptr);
	}

	// Cap Item
	{
		EquipmentButton* button = new  EquipmentButton(eEquipmentType::Cap);
		button->SetLayerType(eLayerType::UI);
		button->Initalize();


		Transform* buttonTr = button->GetComponent<Transform>();
		buttonTr->SetPosition(Vector3(6.05f, 4.9f, 1.0f));
		buttonTr->SetScale(Vector3(1.f, 1.0f, 0.0f));
		button->OnActive();

		SetChild(button);
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
