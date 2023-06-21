#pragma once
#include "CUiBase.h"
#include "CSkilSelectButton.h"

class SkilSelectPanel : public UiBase
{
public:
	SkilSelectPanel();
	~SkilSelectPanel();

	virtual void Initalize() override;
	virtual void Update() override;
	virtual void FixedUpdate() override;
	virtual void Render() override;


	virtual void OnActive() override;
	virtual void UnActive() override;

	void ResetTex();
	
	void SetCurButton(SkilSelectButton* button) { mCurButton = button; }
	SkilSelectButton* GetCurButton() { return mCurButton; }

private:
	std::vector<SkilSelectButton*> mSelectButton[3];
	SkilSelectButton* mCurButton;

	std::shared_ptr<Material> mMater;
};

