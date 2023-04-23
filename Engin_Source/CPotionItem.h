#pragma once
#include "CItemBase.h"

class PotionItem : public ItemBase
{
public:
	PotionItem();
	virtual ~PotionItem();

	virtual void Initalize() override {};
	virtual void Update() override;
	virtual void FixedUpdate() override;
	virtual void Render() override;

	virtual void InitAnimation() override {};
private:

};

