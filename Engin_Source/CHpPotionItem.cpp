#include "CHpPotionItem.h"

HpPotionItem::HpPotionItem()
	: PotionItem()
{
	SetFill(20);
}

HpPotionItem::~HpPotionItem()
{
}

void HpPotionItem::Initalize()
{
}

void HpPotionItem::UsePotion()
{
	GameObject* object = GetTargetObject();
	if (object)
	{
		// 오브젝트의 체력 회복
		// int hp = object->GetHp();
		// hp += GetFIll();
		// object->SetHP(hp);
		this->Death();
	}
}
