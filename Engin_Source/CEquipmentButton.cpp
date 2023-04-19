#include "CEquipmentButton.h"

EquipmentButton::EquipmentButton(eEquipmentType type)
	: Button(eUIType::Button)
	, mType(type)
{
}

EquipmentButton::~EquipmentButton()
{
}

void EquipmentButton::Initalize()
{
	Button::Initalize();
}

void EquipmentButton::Update()
{
	Button::Update();
}

void EquipmentButton::FixedUpdate()
{
	Button::FixedUpdate();
}

void EquipmentButton::Render()
{
	Button::Render();
}

void EquipmentButton::Click()
{
}
