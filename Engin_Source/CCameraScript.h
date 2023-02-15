#pragma once
#include "CScript.h"

class CameraScript : public Script
{
public:
	CameraScript();
	~CameraScript();

	virtual void Initalize() override;
	virtual void Update() override;
	virtual void FixedUpdate() override;
	virtual void Render() override;

private:

};

