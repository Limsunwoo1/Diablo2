#pragma once
#include "CPanel.h"
#include "CEquipmentButton.h"

using namespace std;
class MainPanel : public Panel
{
public:
	MainPanel();
	virtual ~MainPanel();

	virtual void Update() override;
	virtual void FixedUpdate() override;

	void InitPotionSlot();
	void UsePotion();
private:
	vector<EquipmentButton*> mPotionSlot;
};

