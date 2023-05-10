#include "CParticleSystem.h"
#include "CMesh.h"
#include "CResourceManager.h"
#include "CMaterial.h"
#include "CStructedBuffer.h"
#include "CGameObject.h"
#include "CTexture2D.h"
#include "CParticleShader.h"
#include "CTime.h"

using namespace graphics;

ParticleSystem::ParticleSystem()
	: BaseRenderer(eComponentType::ParticleSystem)
	, mBuffer(nullptr)
	, mMaxParticles(100)
	, mStartSize(Vector4(50.f, 50.f, 1.0f, 1.0f))
	, mStartColor(Vector4(1.0f, 0.2f, 0.2f, 1.0f))
	, mStartLifeTime(3.0f)
	, mFrequency(1.0f)
	, mTime(0.0f)
	, mCBData{}
	, mSimulationSpace(eSimulationSpace::World)
	, mRadius(500.f)
	, mStartSpeed(200.f)
	, mElapsedTime(0.0f)
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

	weak_ptr<Mesh> pointrect = ResourceManager::GetInstance()->Find<Mesh>(L"PointMesh");
	this->SetMesh(pointrect);

	// Material
	weak_ptr<Material> material = ResourceManager::GetInstance()->Find<Material>(L"ParticleMaterial");
	this->SetMaterial(material);

	weak_ptr<graphics::Texture2D> tex = ResourceManager::GetInstance()->Find<graphics::Texture2D>(L"CartoonSmoke");
	material.lock()->SetTexture(eTextureSlot::T0, tex);


	Particle particles[100] = {};
	for (size_t i = 0; i < mMaxParticles; i++)
	{
		particles[i].position = Vector4(0.0f, 0.0f, 20.f, 1.0f);
		particles[i].active = 0;
		particles[i].direction = Vector4(cosf((float)i * (XM_2PI / (float)mMaxParticles))
			, sin((float)i * (XM_2PI / (float)mMaxParticles)), 0.0f, 1.0f);

		particles[i].speed = 1.0f;

	}

	mBuffer = new StructedBuffer();
	mBuffer->Create(sizeof(Particle), mMaxParticles, eSRVType::UAV, particles);

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

	mMaxParticles = mBuffer->GetStride();
	Vector3 pos = GetOwner()->GetComponent<Transform>()->GetPosition();
	mCBData.worldPosition = Vector4(pos.x, pos.y, pos.z,1.0f);
	mCBData.maxParticles = mMaxParticles;
	mCBData.radius = mRadius;
	mCBData.simulationSpace = (UINT)mSimulationSpace;
	mCBData.startSpeed = mStartSpeed;
	mCBData.startSize = mStartSize;
	mCBData.startColor = mStartColor;
	mCBData.startLifeTime = mStartLifeTime;
	mCBData.deltaTime = Time::GetInstance()->DeltaTime();
	mCBData.elapsedTime += Time::GetInstance()->DeltaTime();

	ConstantBuffer* cb = Renderer::constantBuffers[(UINT)eCBType::ParticleSystem];
	cb->SetData(&mCBData);
	cb->Bind(eShaderStage::ALL);

	mCS.lock()->SetShaderStructedBuffer(mShaderBuffer);
	mCS.lock()->SetStructedBuffer(mBuffer);
	mCS.lock()->OnExcute();
}

void ParticleSystem::Render()
{
	GetOwner()->GetComponent<Transform>()->SetConstantBuffer();

	mBuffer->BindSRV(eShaderStage::GS, 15);

	GetMaterial().lock()->Bind();
	GetMesh().lock()->RenderInstanced(mMaxParticles);

	mBuffer->Clear();
}
