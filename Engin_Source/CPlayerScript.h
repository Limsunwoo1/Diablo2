#pragma once
#include "CScript.h"
#include "CAStart.h"

using namespace Math;
class PlayerScript : public Script
{
public:
	PlayerScript();
	virtual ~PlayerScript();

	virtual void Initalize() override;
	virtual void Update() override;
	virtual void FixedUpdate() override;
	virtual void Render() override;

	void ResetAStar();

	void Start();
	void Action();
	void End();

	void Idle();
	void Move();

	float GetAngle(Vector2 point);
	
private:
	Vector3 mArrivePos;
	Vector2 mPickPoint;
	Vector2 mEndPos;

	AStar::Node* mNode;
	bool mbInput;
	float mInputDelay;
};

