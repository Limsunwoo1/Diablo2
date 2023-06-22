#pragma once
#include "CGameObject.h"


class TriggerObject : public GameObject
{
public:
	TriggerObject();
	~TriggerObject();

	virtual void Initalize();
	virtual void Update();
	virtual void FixedUpdate();
	virtual void Render();
	void SetTarget(GameObject* target) { mTarget = target; }

	std::function<void()>& GetFunc() { return mFunc; }
private:
	std::function<void()> mFunc;
	GameObject* mTarget;
};

