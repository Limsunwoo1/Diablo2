#pragma once
#include "CScript.h"

class PlayerScript : public Script
{
public:
	PlayerScript();
	~PlayerScript();

	virtual void Initalize() override;
	virtual void Update() override;
	virtual void FixedUpdate() override;
	virtual void Render() override;

	
private:

};

