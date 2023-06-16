#include "CFrozenBolt.h"
#include "CAnimator.h"
#include "CResourceManager.h"
#include "CTransform.h"
#include "CSpriteRenderer.h"
#include "CObject.h"
#include "CLight.h"

FrozenBolt::FrozenBolt()
	: BoltBase()
{
	SetElementType(eElementType::HitFrozen);
}

FrozenBolt::~FrozenBolt()
{
}

void FrozenBolt::Initalize()
{
	// 사이즈
	Transform* tr = GetComponent<Transform>();
	tr->SetSize(Vector3(320.f, 320.f, 1.0f));

	// 애니메이터 초기화
	InitAnimation();

	// 렌더러
	SpriteRenderer* sr = AddComponent<SpriteRenderer>();
	weak_ptr<Mesh> mesh = ResourceManager::GetInstance()->Find<Mesh>(L"RectMesh");
	weak_ptr<Material> material = ResourceManager::GetInstance()->Find<Material>(L"FrozenBoltMaterial");
	weak_ptr<Texture2D> tex = ResourceManager::GetInstance()->Find<Texture2D>(L"FrozenBolt");
	material.lock()->SetTexture(eTextureSlot::T0, tex);

	sr->SetMesh(mesh);
	sr->SetMaterial(material);
}

void FrozenBolt::Update()
{
	BoltBase::Update();
}

void FrozenBolt::FixedUpdate()
{
	BoltBase::FixedUpdate();
}

void FrozenBolt::Render()
{
	BoltBase::Render();
}

void FrozenBolt::InitAnimation()
{
	Animator* animator = AddComponent<Animator>();

	weak_ptr<Texture2D> tex =
		ResourceManager::GetInstance()->Load<Texture2D>(L"FrozenBolt", L"FrozenBolt//FrozenBolt.png");

	animator->Create(L"FrozenBolt", tex, Vector2::Zero, Vector2(45.f, 59.f), Vector2::Zero, 5, 0.1f);
	animator->Play(L"FrozenBolt");
}
