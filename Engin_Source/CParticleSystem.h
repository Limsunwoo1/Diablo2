#pragma once
#include "CBaseRenderer.h"
#include "CParticleShader.h"

using namespace std;
using namespace graphics;

//class ParticleShader;

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
	class StructedBuffer* mShaderBuffer;

	shared_ptr<ParticleShader> mCS;

	UINT mCount;
	Vector4 mStartSize;
	Vector4 mEndSize;
	Vector4 mStartColor;
	Vector4 mEndColor;
	float mStartLifeTime;
	float mFrequency;
	float mTime;
};
