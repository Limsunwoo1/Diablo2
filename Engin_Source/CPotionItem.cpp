#include "CPotionItem.h"

PotionItem::PotionItem()
	: ItemBase(eEquipmentType::Potion)
	, mFillGauge(0)
{
	SetItemSlotSize(Vector2(1.0f, 1.0f));
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
