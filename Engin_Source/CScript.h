#pragma once
#include "CComponent.h"


class Script : public Component
{
public:
	Script();
	virtual ~Script();

	virtual void Initalize() {};
	virtual void Update() {};
	virtual void FixedeUpdate() {};
	virtual void Render() {};

private:
	

};

