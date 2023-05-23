#include "CPotionItem.h"

PotionItem::PotionItem()
	: ItemBase(eEquipmentType::Potion)
	, mFillGauge(0)
{
	SetItemSlotSize(Vector2(1.0f, 1.0f));
	SetWorldSize(Vector3(3.0f * ITEM_X_SIZE, 3.0f * ITEM_Y_SIZE, 0.0f));
	SetInvenSize(Vector3(ITEM_X_SIZE * 1.0f, ITEM_Y_SIZE * 1.0f, 0.0f));
}

PotionItem::~PotionItem()
{
}

void PotionItem::Initalize()
{
}

void PotionItem::Update()
{
	ItemBase::Update();
}

void PotionItem::FixedUpdate()
{
	ItemBase::FixedUpdate();
}

void PotionItem::Render()
{
	ItemBase::Render();
}

void PotionItem::InitAnimation()
{
}
