#include "CFlames.h"
#include "CSpriteRenderer.h"
#include "CAnimator.h"

#include "CResourceManager.h"


Flames::Flames()
	: Skil()
{
}

Flames::~Flames()
{
}

void Flames::Initalize()
{
	InitAnimation();

	// 사이즈
	Transform* tr = GetComponent<Transform>();
	tr->SetScale(Vector3(4.0f, 2.0f, 1.0f));
	tr->SetRotation(Vector3(0.f, 0.f, 0.f));

	// 렌더러
	SpriteRenderer* sr = AddComponent<SpriteRenderer>();
	shared_ptr<Mesh> mesh = ResourceManager::GetInstance()->Find<Mesh>(L"HalfAlphaMesh");
	shared_ptr<Material> material = ResourceManager::GetInstance()->Find<Material>(L"FlameMaterial");
	shared_ptr<Texture2D> tex = ResourceManager::GetInstance()->Find<Texture2D>(L"Flame");
	material->SetTexture(eTextureSlot::T0, tex);
	tex = ResourceManager::GetInstance()->Load<Texture2D>(L"FlameBoom", L"Meteor//MeteorBoom.png");
	material->SetTexture(eTextureSlot::T2, tex);
	
	sr->SetMesh(mesh);
	sr->SetMaterial(material);

	Death();
}

void Flames::Update()
{
	Skil::Update();
}

void Flames::FixedUpdate()
{
	Skil::FixedUpdate();
}

void Flames::Render()
{
	Skil::Render();
}

void Flames::InitAnimation()
{
	shared_ptr<Texture2D> tex =
		ResourceManager::GetInstance()->Load<Texture2D>(L"Flame", L"Meteor//FlamesSprite2.png");

	Animator* animator = AddComponent<Animator>();
	animator->Create(L"Flame", tex, Vector2::Zero, Vector2(100.f, 100.f), Vector2::Zero, 20, 0.1f);

	tex = ResourceManager::GetInstance()->Load<Texture2D>(L"FlameBoom", L"Meteor//MeteorBoom.png");
	animator->Create(L"FlameBoom", tex, Vector2::Zero, Vector2(100.f, 100.f), Vector2::Zero, 19, 0.1f);

	animator->Play(L"FlameBoom", false);
}
