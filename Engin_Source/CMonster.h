#pragma once
#include "CScript.h"

class Monster : public Script
{
public:
	Monster();
	virtual ~Monster();

	virtual void Initalize();
	virtual void Update();
	virtual void FixedUpdate();
	virtual void Render();

private:

	
};

