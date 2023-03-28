#include "CParticleSystem.h"
#include "CMesh.h"
#include "CResourceManager.h"
#include "CMaterial.h"
#include "CStructedBuffer.h"
#include "CTransform.h"
#include "CGameObject.h"


ParticleSystem::ParticleSystem()
	: BaseRenderer(eComponentType::ParticleSystem)
	, mBuffer(nullptr)
	, mCount(0)
	, mStartSize(Vector4::Zero)
	, mEndSize(Vector4::Zero)
	, mStartColor(Vector4::Zero)
	, mEndColor(Vector4::Zero)
	, mStartLifeTime(0.0f)
{
	shared_ptr<Mesh> rect = ResourceManager::GetInstance()->Find<Mesh>(L"RectMesh");
	this->SetMesh(rect);

	// Material
	shared_ptr<Material> material = ResourceManager::GetInstance()->Find<Material>(L"ParticleMaterial");
	this->SetMaterial(material);

	Particle particles[1000] = {};
	Vector4 startePos = Vector4(-800.f, -450.f, 0.0f, 0.0f);
	for (size_t y = 0; y < 9; y++)
	{
		for (size_t x = 0; x < 16; x++)
		{
			particles[16 * y + x].position = startePos
				+ Vector4(x * 100.0f, y * 100.0f, 0.0f, 0.0f);
		}
	}

	mCount = 144;
	mBuffer = new StructedBuffer();
	mBuffer->Create(sizeof(Particle), mCount, eSRVType::None, particles);
} 

ParticleSystem::~ParticleSystem()
{
	delete mBuffer;
	mBuffer = nullptr;
}

void ParticleSystem::Initalize()
{
}

void ParticleSystem::Update()
{
}

void ParticleSystem::FixedUpdate()
{
}

void ParticleSystem::Render()
{
	GetOwner()->GetComponent<Transform>()->SetConstantBuffer();
	mBuffer->Bind(eShaderStage::VS, 15);
	mBuffer->Bind(eShaderStage::PS, 15);

	GetMaterial()->Bind();
	GetMesh()->RenderInstanced(mCount);
}
