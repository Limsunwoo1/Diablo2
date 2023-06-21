#pragma once
#include "CUiBase.h"

class Button : public UiBase
{
public:
	Button(eUIType type);
	virtual ~Button();

	virtual void OnInit() override;
	virtual void OnActive() override;
	virtual void UnActive() override;
	virtual void OnUpdate() override;
	virtual void OnRender(HDC hdc) override;
	virtual void OnClear() override;

	virtual void Initalize() override;
	virtual void Update() override;
	virtual void FixedUpdate() override;
	virtual void Render() override;

	virtual void InitAnimation() override;
	virtual void InitRenderer(const wstring& materialName
		, const wstring& textureName, const std::wstring& textruepath) override;

	virtual void Click();
	virtual void BindEvnet(std::function<void()> fun);
	virtual void OnEvent();

	void SetChildRun(bool run) { mbChildRun = run; }
	void SetChild(Button* child) { mChild = child; }
	Button* GetChild() { return mChild; }

protected:
	Button* mChild;
	bool mbChildRun;

private:
	Event mOnClick;
	bool mbMouseOn;

};

