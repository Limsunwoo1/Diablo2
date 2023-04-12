#pragma once
#include "CScript.h"

class MonsterScript : public Script
{
public:
	MonsterScript();
	virtual ~MonsterScript();

	virtual void Initalize();
	virtual void Update();
	virtual void FixedUpdate();
	virtual void Render();

private:

	
};

