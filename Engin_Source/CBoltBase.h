#pragma once
#include "CSkil.h"

class BoltBase : public Skil
{
public:
	BoltBase();
	virtual ~BoltBase();

	virtual void Update() override;

	void SetDirection(float angle);
	float Angle(Vector2 point);

	bool GetRun() { return mbRun; }
	void SetRun(bool run) { mbRun = run; }

	float GetDamage() { return mDameage; }
	void SetDamage(float damage) { mDameage = damage; }

	void SetSpeed(int speed) { mSpeed = speed; }
private:
	int mSpeed;

	float mDirection;
	float mDameage;

	bool mbRun;
};

