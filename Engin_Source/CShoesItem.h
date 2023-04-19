#pragma once
#include "CItemBase.h"

using namespace std;
class ShoesItem : public ItemBase
{
public:
	ShoesItem(const wstring& name);
	virtual ~ShoesItem();

	virtual void Initalize() override;

private:
};

