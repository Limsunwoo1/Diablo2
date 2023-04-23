#pragma once
#include "CItemBase.h"

using namespace std;
class BeltItem : public ItemBase
{
public:
	BeltItem(const wstring& name);
	virtual ~BeltItem();

	virtual void Initalize() override;

private:
};

