#pragma once
#include "CItemBase.h"

using namespace std;
class GloveItem : public ItemBase
{
public:
	GloveItem(const wstring& name);
	virtual ~GloveItem();

	virtual void Initalize() override;

private:
};

