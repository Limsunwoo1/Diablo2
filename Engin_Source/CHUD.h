#pragma once
#include "CUiBase.h"


class HUD : public UiBase
{
public:
	HUD(eUIType type);
	~HUD();

	virtual void OnInit() override;
	virtual void OnActive() override;
	virtual void OnInActive() override;
	virtual void OnTick() override;
	virtual void OnRender(HDC hdc) override;
	virtual void OnClear() override;

private:

	bool mbMouseOn;
};


