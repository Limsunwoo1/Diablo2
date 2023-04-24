#include "CMpPotionItem.h"

MpPotionItem::MpPotionItem()
	: PotionItem()
{
	SetFill(20);
}

MpPotionItem::~MpPotionItem()
{
}

void MpPotionItem::Initalize()
{
}

void MpPotionItem::UsePotion()
{
	GameObject* object = GetTargetObject();
	if (object)
	{
		// 오브젝트의 마나 회복
		this->Death();
	}
}
