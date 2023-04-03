#include "CParticleShader.h"
#include "CRenderer.h"
#include "CConstantBuffer.h"
#include "CTime.h"


graphics::ParticleShader::ParticleShader()
	: ComputeShader(128, 1, 1)
	, mBuffer(nullptr)
	, mShaderBuffer(nullptr)
{
}

graphics::ParticleShader::~ParticleShader()
{
}

void graphics::ParticleShader::Binds()
{
	mBuffer->BindUAV(eShaderStage::CS, 0);
	mShaderBuffer->BindUAV(eShaderStage::CS, 1);

	mGroupX = mBuffer->GetStride() / mThreadGropCountX + 1;
	mGroupY = 1;
	mGroupZ = 1;
}

void graphics::ParticleShader::Clear()
{
	mBuffer->Clear();
	mShaderBuffer->Clear();
}
