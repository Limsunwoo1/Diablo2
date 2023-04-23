#pragma once
#include "CItemBase.h"

using namespace std;
class SuitItem : public ItemBase
{
public:
	SuitItem(const wstring& name);
	virtual ~SuitItem();

	virtual void Initalize() override;

private:
};

