#include "CPotionItem.h"

PotionItem::PotionItem()
	: ItemBase(eEquipmentType::Potion)
	, mFillGauge(0)
{
	SetItemSlotSize(Vector2(1.0f, 1.0f));
	SetWorldSize(Vector3(3.0f * ITEM_X_SIZE * 2.0f, 3.0f * ITEM_Y_SIZE * 2.0f, 0.0f));
	SetInvenSize(Vector3(ITEM_X_SIZE * 1.0f, ITEM_Y_SIZE * 1.0f, 0.0f));

	ItemBase::Initalize();
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
