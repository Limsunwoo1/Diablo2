#pragma once
#include "CUiBase.h"


class GameObject;
class Panel : public UiBase
{
public:
	Panel(eUIType type);
	~Panel();

	virtual void OnInit() override;
	virtual void OnActive() override;
	virtual void OnInActive() override;
	virtual void OnTick() override;
	virtual void OnRender(HDC hdc) override;
	virtual void OnClear() override;

	void SetChild(Vector2 vector, UiBase* child);
	void SetTarget(GameObject* target) { mTarget = target; }
public:
	std::vector<pair<Vector2, UiBase*>> mChilds;
	GameObject* mTarget;
};

