#include "CFireTargetPin.h"
#include "CAnimator.h"
#include "CCollider2D.h"
#include "CSpriteRenderer.h"
#include "CMaterial.h"
#include "CMesh.h"
#include "CResourceManager.h"
#include "CGenericAnimator.h"

FireTargetPin::FireTargetPin(Player* player)
	: Skil(player)
{
}

FireTargetPin::FireTargetPin()
	: Skil()
{
}

FireTargetPin::~FireTargetPin()
{
}

void FireTargetPin::Initalize()
{
	// Size
	Transform* tr = GetComponent<Transform>();
	tr->SetScale(Vector3(3.0f, 2.0f, 1.0f));

	// 애니메이터
	InitAnimation();

	// 제네릭 애니메이터
	AddComponent<GenericAnimator>();

	// renderer
	SpriteRenderer* sr = AddComponent<SpriteRenderer>();
	std::weak_ptr<Mesh> mesh = ResourceManager::GetInstance()->Find<Mesh>(L"RectMesh");
	std::weak_ptr<Material> material = ResourceManager::GetInstance()->Find<Material>(L"FirePinMaterial");
	std::weak_ptr<Texture2D> tex = ResourceManager::GetInstance()->Find<Texture2D>(L"FirePin");
	sr->SetMesh(mesh);
	sr->SetMaterial(material);
}

void FireTargetPin::Update()
{
	Skil::Update();
}

void FireTargetPin::FixedUpdate()
{
	Skil::FixedUpdate();
}

void FireTargetPin::Render()
{
	Skil::Render();
}

void FireTargetPin::InitAnimation()
{
	std::weak_ptr<Texture2D> tex = ResourceManager::GetInstance()->Load<Texture2D>(L"FirePin", L"Meteor//MeteorPin.png");
	Animator* animator = AddComponent<Animator>();

	animator->Create(L"FirePin", tex, Vector2::Zero, Vector2(100.f, 100.f), Vector2::Zero, 17, 0.02f);
	animator->Play(L"FirePin");
}
