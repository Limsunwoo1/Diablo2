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
		// ������Ʈ�� ü�� ȸ��
		// int hp = object->GetHp();
		// hp += GetFIll();
		// object->SetHP(hp);
		this->Death();
	}
}
