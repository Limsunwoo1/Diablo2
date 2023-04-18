#include "CInventoryPanel.h"
#include "CButton.h"
#include "CObject.h"

InventoryPanel::InventoryPanel()
	: Panel(eUIType::Panel)
{
}

InventoryPanel::~InventoryPanel()
{
}

void InventoryPanel::Initalize()
{
	Button* button = new  Button(eUIType::Button);
	button->SetLayerType(eLayerType::UI);
	button->Initalize();

	Transform* buttonTr = button->GetComponent<Transform>();
	buttonTr->SetPosition(Vector3(9.f, -0.5f, 1.0f));
	buttonTr->SetScale(Vector3(6.4f, 2.1f, 0.0f));
	button->OnActive();


	SetChild(button);
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
