#pragma once
#include "CScript.h"
#include "CAStar.h"

using namespace Math;

class MinoMonsterScript : public Script
{
public:
	MinoMonsterScript();
	virtual ~MinoMonsterScript();

	virtual void Initalize();
	virtual void Update();
	virtual void FixedUpdate();
	virtual void Render();

	float GetAngle(Vector2& direction);

private:
	Vector2 mCurPos;
	AStar::Node* mNode;

	float mTime;
};

