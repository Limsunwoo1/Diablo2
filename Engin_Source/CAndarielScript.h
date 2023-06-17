#pragma once
#include "CScript.h"
#include "CAStar.h"

using namespace Math;

class AndarielScript : public Script
{
public:
	AndarielScript();
	virtual ~AndarielScript();

	virtual void Initalize();
	virtual void Update();
	virtual void FixedUpdate();
	virtual void Render();

	float GetAngle(Vector2& direction);
	virtual void ResetAStar() override;

private:
	Vector2 mCurPos;
};
