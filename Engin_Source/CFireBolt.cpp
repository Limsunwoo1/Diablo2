#include "CFireBolt.h"
#include "CAnimator.h"
#include "CResourceManager.h"
#include "CTransform.h"
#include "CSpriteRenderer.h"
#include "CObject.h"
#include "CLight.h"

FireBolt::FireBolt()
	: BoltBase()
{
	SetElementType(eElementType::HitFire);
}

FireBolt::~FireBolt()
{
}

void FireBolt::Initalize()
{
	// 사이즈
	Transform* tr = GetComponent<Transform>();
	tr->SetSize(Vector3(550.f, 300.f, 1.0f));

	// 애니메이터 초기화
	InitAnimation();

	// 렌더러
	SpriteRenderer* sr = AddComponent<SpriteRenderer>();
	weak_ptr<Mesh> mesh = ResourceManager::GetInstance()->Find<Mesh>(L"RectMesh");
	weak_ptr<Material> material = ResourceManager::GetInstance()->Find<Material>(L"FireBoltMaterial");
	weak_ptr<Texture2D> tex = ResourceManager::GetInstance()->Find<Texture2D>(L"FireBolt");
	material.lock()->SetTexture(eTextureSlot::T0, tex);

	sr->SetMesh(mesh);
	sr->SetMaterial(material);
}

void FireBolt::Update()
{
	BoltBase::Update();
}

void FireBolt::FixedUpdate()
{
	BoltBase::FixedUpdate();
}

void FireBolt::Render()
{
	BoltBase::Render();
}

void FireBolt::InitAnimation()
{
	Animator* animator = AddComponent<Animator>();
	weak_ptr<Texture2D> tex =
		ResourceManager::GetInstance()->Load<Texture2D>(L"FireBolt", L"FirBolt//FirteBolt2.png");


	animator->Create(L"FireBolt", tex, Vector2::Zero, Vector2(100.f, 100.f), Vector2::Zero, 9, 0.1f);
	animator->Play(L"FireBolt");
}
