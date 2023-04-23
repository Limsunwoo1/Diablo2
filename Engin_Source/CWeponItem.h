#pragma once
#include "CItemBase.h"

using namespace std;
class WeponItem : public ItemBase
{
public:
	WeponItem(const wstring& name);
	virtual ~WeponItem();

	virtual void Initalize() override;

private:
};

