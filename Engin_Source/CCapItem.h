#pragma once
#include "CItemBase.h"

using namespace std;
class CapItem : public ItemBase
{
public:
	CapItem(const wstring& name);
	virtual ~CapItem();

	virtual void Initalize() override;

private:
};

