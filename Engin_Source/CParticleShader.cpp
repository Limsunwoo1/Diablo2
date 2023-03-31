#include "CParticleShader.h"
#include "CRenderer.h"
#include "CConstantBuffer.h"
#include "CTime.h"


graphics::ParticleShader::ParticleShader()
	: ComputeShader(128, 1, 1)
	, mBuffer(nullptr)
{
}

graphics::ParticleShader::~ParticleShader()
{
}

void graphics::ParticleShader::Binds()
{
	mBuffer->BindUAV(eShaderStage::CS, 0);

	mGroupX = mBuffer->GetStride() / mThreadGropCountX + 1;
	mGroupY = 1;
	mGroupZ = 1;
}

void graphics::ParticleShader::Clear()
{
	mBuffer->Clear();
}

void graphics::ParticleShader::SetStructedBuffer(StructedBuffer* buffer)
{
	mBuffer = buffer;

	Renderer::ParticleSystemCB info = {};
	info.elementCount = mBuffer->GetStride();
	info.delta = Time::GetInstance()->DeltaTime();

	ConstantBuffer* ct = Renderer::constantBuffers[(UINT)eCBType::ParticleSystem];

	ct->SetData(&info);
	ct->Bind(eShaderStage::CS);
}
