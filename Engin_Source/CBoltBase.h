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

	void SetSpeed(int speed) { mSpeed = speed; }

	virtual void HitSkil(GameObject* obj) override;
private:
	int mSpeed;

	float mDirection;

	bool mbRun;
};

