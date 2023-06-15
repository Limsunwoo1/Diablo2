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
private:
	int mSpeed;

	float mDirection;
	float mDameage;

	bool mbRun;
};

