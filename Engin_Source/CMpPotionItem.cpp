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
		// ������Ʈ�� ���� ȸ��
		this->Death();
	}
}
