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

	void CreateButton();
	void RemoveButton();
	void RetrunSceneButton();
	void Select_Ok_Button();

	void ButtonPositionInit();
	void CreateSelectButton();

private:
	vector<PlayerSelectButton*> mButton;
	PlayerSelectButton* mClickButton;

	class SelectSystemButton* mCreateButton;
	class SelectSystemButton* mReturnSceneButton;
	class SelectSystemButton* mSelect_Ok_Button;

	int mClickButtonIdx;
};

