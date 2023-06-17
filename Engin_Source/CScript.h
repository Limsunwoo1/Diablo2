#pragma once
#include "CComponent.h"

class Collider2D;
class Script : public Component
{
public:
	Script();
	virtual ~Script();

	virtual void Initalize() {};
	virtual void Update() {};
	virtual void FixedeUpdate() {};
	virtual void Render() {};

	virtual void OnCollisionEnter(Collider2D* collider) {};
	virtual void OnCollisionStay(Collider2D* collider) {};
	virtual void OnCollisionExit(Collider2D* collider) {};

	virtual void OnTriggerEnter(Collider2D* collider) {};
	virtual void OnTriggerStay(Collider2D* collider) {};
	virtual void OnTriggerExit(Collider2D* collider) {};

	void SetPosData(std::stack<Math::Vector2>& pos);
	virtual void ResetAStar() {};

	void SetTime(float time) { mTime = time; }
	void SetMaxTime(float time) { mMaxTime = time; }
protected:
	std::stack<Math::Vector2> mPosData;
	bool mbInput;
	float mInputDelay;
	float mTime;
	float mMaxTime;


	Vector2 mNodePos;
	Vector2 mArrivePos;
};

