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

	void InventorySetting(UINT index);
	void Dynamic_ItemSetting(UINT type, UINT stage, UINT x, UINT y);
	void SlotItemSetting(class ItemBase* item);
private:
	vector<PlayerSelectButton*> mButton;
	vector<wstring> mItemData;
	PlayerSelectButton* mClickButton;

	class SelectSystemButton* mCreateButton;
	class SelectSystemButton* mReturnSceneButton;
	class SelectSystemButton* mSelect_Ok_Button;

	int mClickButtonIdx;
};

