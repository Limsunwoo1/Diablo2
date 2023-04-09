#pragma once
#include "CBaseRenderer.h"
#include "CParticleShader.h"

using namespace std;
using namespace graphics;

//class ParticleShader;

enum class eSimulationSpace
{
	Local,
	World,
};

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
	Renderer::ParticleSystemCB mCBData;

	Vector4 mStartSize;
	Vector4 mStartColor;

	eSimulationSpace mSimulationSpace;
	UINT mMaxParticles;
	float mStartLifeTime;
	float mFrequency;
	float mRadius;

	float mStartSpeed;
	float mTime;
	float mElapsedTime; // 누적시간
};

