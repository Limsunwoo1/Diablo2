#pragma once
#include "CUiBase.h"

class Button : public UiBase
{
public:
	Button(eUIType type);
	~Button();

	virtual void OnInit() override;
	virtual void OnActive() override;
	virtual void OnInActive() override;
	virtual void OnTick() override;
	virtual void OnRender(HDC hdc) override;
	virtual void OnClear() override;

	virtual void Click();

private:
	Event mOnClick;
	bool mbMouseOn;
};

