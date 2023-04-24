#pragma once
#include "CPotionItem.h"

class HpPotionItem : public PotionItem
{
public:
	HpPotionItem();
	virtual ~HpPotionItem();

	virtual void Initalize() override;
	virtual void UsePotion() override;
private:

};

