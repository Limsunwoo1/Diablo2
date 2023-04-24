#include "CPotionItem.h"

PotionItem::PotionItem()
	: ItemBase(eEquipmentType::Potion)
	, mTargetObject(nullptr)
	, mFillGauge(0)
{
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
