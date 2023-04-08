#include "CFrozenOrb.h"
#include "CFrozenMisile.h"
#include "CAnimator.h"
#include "CSpriteRenderer.h"

#include "CTime.h"
#include "CResourceManager.h"

FrozenOrb::FrozenOrb()
	: Skil()
	, mFrozenMisile{}
{
}

FrozenOrb::~FrozenOrb()
{
}

void FrozenOrb::Initalize()
{
	/*for (int i = 0; i < 50; ++i)
		mFrozenMisile.emplace_back(new FrozenMisile);*/

	// �� �� �� �� �� ��
	mMisileDirection.push_back(Vector2(1.f, -1.f));
	mMisileDirection.push_back(Vector2(-1.f, 1.f));
	mMisileDirection.push_back(Vector2(1.f, 1.f));
	mMisileDirection.push_back(Vector2(-1.f, -1.f));
	mMisileDirection.push_back(Vector2(1.f, 0.f));
	mMisileDirection.push_back(Vector2(-1.f, 0.f));

	InitAnimation();

	// ������
	SpriteRenderer* sr = AddComponent<SpriteRenderer>();
	shared_ptr<Mesh> mesh = ResourceManager::GetInstance()->Find<Mesh>(L"RectMesh");
	shared_ptr <Material> material = ResourceManager::GetInstance()->Find<Material>(L"FrozenOrbMaterial");
	sr->SetMesh(mesh);
	sr->SetMaterial(material);

	shared_ptr<Texture2D> tex = ResourceManager::GetInstance()->Find<Texture2D>(L"FrozenOrb");
	material->SetTexture(eTextureSlot::T0,tex);

	// Ʈ������
	Transform* tr = GetComponent<Transform>();
	tr->SetScale(Vector3(2.f, 2.f, 1.f));
}

void FrozenOrb::Update()
{
	Skil::Update();
}

void FrozenOrb::FixedUpdate()
{
	RunOrb();
	Skil::FixedUpdate();
}

void FrozenOrb::Render()
{
	Skil::Render();
}

void FrozenOrb::InitAnimation()
{
	Animator* animator = AddComponent<Animator>();
	shared_ptr<Texture2D> tex = ResourceManager::GetInstance()->Load<Texture2D>(L"FrozenOrb", L"Frozen//FrozenOrb.png");

	animator->Create(L"FrozenOrb", tex, Vector2(0.0f, 0.0f), Vector2(100.f, 100.f), Vector2(0.0f, 0.0f), 16, 0.1f);
	animator->Play(L"FrozenOrb");
}

void FrozenOrb::RunOrb()
{
	Transform* tr = GetComponent<Transform>();
	Vector3 pos = tr->GetPosition();

	mDirection.Normalize();
	pos += Vector3(mDirection.x, mDirection.y, 0.0f) * Time::GetInstance()->DeltaTime() * 2.f;

	tr->SetPosition(pos);
}
