#pragma once
#include "..//Engin_Source/CGameObject.h"

class DebugObject : public GameObject
{
public:
	DebugObject();
	virtual ~DebugObject();

	virtual void FixedUpdate() override;
private:

};

