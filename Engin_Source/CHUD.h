#pragma once
#include "CUiBase.h"


class HUD : public UiBase
{
public:
	HUD(eUIType type);
	~HUD();

	virtual void OnInit() override;
	virtual void OnActive() override;
	virtual void OnInActive() override;
	virtual void OnUpdate() override;
	virtual void OnRender(HDC hdc) override;
	virtual void OnClear() override;

	virtual void Initalize() override;
	virtual void Update() override;
	virtual void FixedUpdate() override;
	virtual void Render() override;

	virtual void InitAnimation() override;
	virtual void InitRenderer(const wstring& materialName,
		const wstring& textureName, const std::wstring& textruepath) override;

private:

	bool mbMouseOn;
};


