#pragma once
#include "CUiBase.h"

using namespace std;
using namespace Math;

class SelectButtonSystem : public UiBase
{
public:
	SelectButtonSystem();
	virtual ~SelectButtonSystem();

	virtual void Initalize() override;
	virtual void Update() override;
	virtual void FixedUpdate() override;
	virtual void Render() override;

	void SetClickButton(class PlayerSelectButton* button) { mClickButton = button; }
	PlayerSelectButton* GetClickButton() { return mClickButton; }

	void DataEdit(string& str){};

private:
	vector<PlayerSelectButton*> mButton;
	PlayerSelectButton* mClickButton;

	int mClickButtonIdx;
};

