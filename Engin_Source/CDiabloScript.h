#pragma once
#include "CScript.h"
#include "CAStar.h"

using namespace Math;

class DiabloScript : public Script
{
public:
	DiabloScript();
	virtual ~DiabloScript();

	virtual void Initalize();
	virtual void Update();
	virtual void FixedUpdate();
	virtual void Render();

	float GetAngle(Vector2& direction);
	virtual void ResetAStar() override;

private:
	Vector2 mCurPos;
};
