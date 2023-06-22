#pragma once
#include "CScript.h"
#include "CAStar.h"

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

	virtual void ResetAStar() override;

	void Start();
	void Action();
	void End();

	void Idle();
	void Move();
	void SetPlayerDirection();
	void SetPlayerDirection(Vector3& Direction);

	void AddRenderAStar();
	void ClearAstar();

	float GetAngle(Vector2 point);

private:
	void skil1();
	void skil2();
	void skil3();
	void skil4();
	void skil5();
	void skil6();
	void skil7();
	
private:
	Vector2 mEndPos;

	AStar::Node* mNode;
	vector<GameObject*> mRenderObj;

};

