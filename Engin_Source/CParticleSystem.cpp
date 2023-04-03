#include "CParticleSystem.h"
#include "CMesh.h"
#include "CResourceManager.h"
#include "CMaterial.h"
#include "CStructedBuffer.h"
#include "CTransform.h"
#include "CGameObject.h"
#include "CTexture2D.h"
#include "CParticleShader.h"
#include "CTime.h"

using namespace graphics;

ParticleSystem::ParticleSystem()
	: BaseRenderer(eComponentType::ParticleSystem)
	, mBuffer(nullptr)
	, mCount(100)
	, mStartSize(Vector4::Zero)
	, mEndSize(Vector4::Zero)
	, mStartColor(Vector4::Zero)
	, mEndColor(Vector4::Zero)
	, mStartLifeTime(0.0f)
	, mFrequency(1.0f)
	, mTime(0.0f)
{

}

ParticleSystem::~ParticleSystem()
{
	delete mBuffer;
	mBuffer = nullptr;

	delete mShaderBuffer;
	mShaderBuffer = nullptr;
}

void ParticleSystem::Initalize()
{
	mCS = ResourceManager::GetInstance()->Find<ParticleShader>(L"ParticleCS");

	shared_ptr<Mesh> pointrect = ResourceManager::GetInstance()->Find<Mesh>(L"PointMesh");
	this->SetMesh(pointrect);

	// Material
	shared_ptr<Material> material = ResourceManager::GetInstance()->Find<Material>(L"ParticleMaterial");
	this->SetMaterial(material);

	shared_ptr<graphics::Texture2D> tex = ResourceManager::GetInstance()->Find<graphics::Texture2D>(L"CartoonSmoke");
	material->SetTexture(eTextureSlot::T0, tex);


	Particle particles[100] = {};
	Vector4 startPos = Vector4(-800.f, -450.f, 0.0f, 0.0f);
	for (size_t i = 0; i < mCount; i++)
	{
		particles[i].position = Vector4(0.0f, 0.0f, 20.f, 1.0f);
		particles[i].active = 0;
		particles[i].direction = Vector4(cosf((float)i * (XM_2PI / (float)mCount))
			, sin((float)i * (XM_2PI / (float)mCount)), 0.0f, 1.0f);

		particles[i].speed = 1.0f;

	}

	mBuffer = new StructedBuffer();
	mBuffer->Create(sizeof(Particle), mCount, eSRVType::UAV, particles);

	mShaderBuffer = new StructedBuffer();
	mShaderBuffer->Create(sizeof(ParticleShadered), 1, eSRVType::UAV, nullptr, true);
}

void ParticleSystem::Update()
{
}

void ParticleSystem::FixedUpdate()
{
	// 파티클 생성시간
	float aliveTime = 1.0f / mFrequency;

	// 누적시간
	mTime += Time::GetInstance()->DeltaTime();
	if (aliveTime < mTime)
	{
		float f = (mTime / aliveTime);
		UINT iAliveCount = (UINT)f;
		mTime = f - std::floor(f);

		ParticleShadered shadered = { 5, };
		mShaderBuffer->SetData(&shadered, 1);
	}
	else
	{
		ParticleShadered shader = {};
		mShaderBuffer->SetData(&shader, 1);
	}

	Renderer::ParticleSystemCB info = {};
	info.elementCount = mBuffer->GetStride();
	info.delta = Time::GetInstance()->DeltaTime();

	ConstantBuffer* cb = Renderer::constantBuffers[(UINT)eCBType::ParticleSystem];
	cb->SetData(&info);
	cb->Bind(eShaderStage::CS);

	mCS->SetShaderStructedBuffer(mShaderBuffer);
	mCS->SetStructedBuffer(mBuffer);
	mCS->OnExcute();
}

void ParticleSystem::Render()
{
	GetOwner()->GetComponent<Transform>()->SetConstantBuffer();

	mBuffer->BindSRV(eShaderStage::VS, 15);
	mBuffer->BindSRV(eShaderStage::GS, 15);
	mBuffer->BindSRV(eShaderStage::PS, 15);

	GetMaterial()->Bind();
	GetMesh()->RenderInstanced(mCount);

	mBuffer->Clear();
}
