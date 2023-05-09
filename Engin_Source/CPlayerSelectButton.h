#pragma once
#include "CButton.h"


using namespace std;
using namespace Math;

class PlayerSelectButton : public Button
{
public:
	PlayerSelectButton();
	virtual ~PlayerSelectButton();

	virtual void Initalize() override;
	virtual void Update() override;
	virtual void FixedUpdate() override;
	virtual void Render() override;

	eCharType GetCharType() { return (eCharType)mChartType; }
	void SetChrType(eCharType type) { mChartType = (UINT)type; }

	wstring& GetCharName() { return mPlayerName; }
	void SetCharName(const wstring& name) { mPlayerName = name; }

	void SetSystem(class SelectButtonSystem* system) { mSystem = system; }
	SelectButtonSystem* GetSystem() { return mSystem; }

	int GetButtonIndex() { return mButtonIndex; }
	void SetButtonIndex(UINT index) { mButtonIndex = index; }

private:
	GameObject* mCharterAnimation;
	SelectButtonSystem* mSystem;

	UINT mChartType;
	int mButtonIndex;
	wstring mPlayerName;
};

