#pragma once
#include "CUiBase.h"


class GameObject;
class Panel : public UiBase
{
public:
	Panel(eUIType type);
	~Panel();

	virtual void OnClear() override;

public:
	virtual void Initalize() override;
	virtual void Update() override;
	virtual void FixedUpdate() override;
	virtual void Render() override;

	virtual void InitAnimation() override;
	virtual void InitRenderer(const wstring& materialName,
		const wstring& textureName, const std::wstring& textruepath) override;

	void SetTarget(GameObject* target) { mTarget = target; }

	virtual void OnActive() override;
	virtual void UnActive() override;
protected:
	virtual void OnInit() override;
	virtual void OnInActive() override;
	virtual void OnUpdate() override;
	virtual void OnRender(HDC hdc) override;
public:
	GameObject* mTarget;
};

