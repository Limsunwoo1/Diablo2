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

	std::function<void()>& GetFunc() { return mFunc; }
private:
	std::function<void()> mFunc;
};

