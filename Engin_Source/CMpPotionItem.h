#pragma once
#include "CPotionItem.h"

class MpPotionItem : public PotionItem
{
public:
	MpPotionItem();
	virtual ~MpPotionItem();

	virtual void Initalize() override;
	virtual void UsePotion() override;
private:
};

