#pragma once
#include "CPanel.h"

class InventoryPanel : public Panel
{
public:
	InventoryPanel();
	virtual ~InventoryPanel();

	virtual void Initalize() override;
	virtual void Update() override;
	virtual void FixedUpdate() override;
	virtual void Render() override;

	virtual void InitAnimation() override;
private:

};

