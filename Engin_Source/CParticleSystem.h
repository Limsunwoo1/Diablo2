#pragma once
#include "CBaseRenderer.h"

using namespace std;

class ParticleSystem : public BaseRenderer
{
public:
	ParticleSystem();
	virtual ~ParticleSystem();

	virtual void Initalize() override;
	virtual void Update() override;
	virtual void FixedUpdate() override;
	virtual void Render() override;

private:
	class StructedBuffer* mBuffer;

	UINT mCount;
	Vector4 mStartSize;
	Vector4 mEndSize;
	Vector4 mStartColor;
	Vector4 mEndColor;
	float mStartLifeTime;
};

