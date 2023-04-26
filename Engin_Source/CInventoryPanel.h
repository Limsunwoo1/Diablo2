#pragma once
#include "CPanel.h"

class InventoryButton;
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

	virtual void OnActive() override;
	virtual void UnActive() override;

	InventoryButton* GetInventoryArr() { return mInvenArr; }
	void SetInvenArr(InventoryButton* arr) { mInvenArr = arr; }
private:
	InventoryButton* mInvenArr;
};

