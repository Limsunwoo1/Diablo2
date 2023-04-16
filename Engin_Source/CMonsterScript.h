#pragma once
#include "CScript.h"

class MinoMonsterScript : public Script
{
public:
	MinoMonsterScript();
	virtual ~MinoMonsterScript();

	virtual void Initalize();
	virtual void Update();
	virtual void FixedUpdate();
	virtual void Render();

private:

	
};

