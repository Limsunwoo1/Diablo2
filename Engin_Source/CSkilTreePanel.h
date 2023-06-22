#pragma once
#include "CUiBase.h"
#include "CFontHUD.h"

class Button;
class SkilTreePanel : public UiBase
{
public:
	SkilTreePanel();
	virtual ~SkilTreePanel();

	virtual void Initalize() override;
	virtual void Update() override;
	virtual void FixedUpdate() override;
	virtual void Render() override;


	virtual void OnActive() override;
	virtual void UnActive() override;

	void ButtonSelect1();
	void ButtonSelect2();
	void ButtonSelect3();

	void SetPoint(UINT point) { mHavePoint = point; }
	UINT GetPoint() { return mHavePoint; }
	void UpPoint() { mHavePoint++; }

	std::vector<Button*> GetButtonChild(UINT Idx);
	std::vector<eSkilList> GetButtonChildType(UINT Idx);

private:
	Button* mCurButton;
	FontHUD* mFont;

	Button* ButtonList[3];
	
	std::vector<std::weak_ptr<Texture2D>> mSkilTreeTex;
	std::shared_ptr<Material> mMaterial;

	UINT mHavePoint;
};

