#include "CMinosTauros.h"
#include "CAnimator.h"
#include "CSpriteRenderer.h"
#include "CResourceManager.h"

MinosTauros::MinosTauros()
	: Monster()
{
}

MinosTauros::~MinosTauros()
{
}

void MinosTauros::Initalize()
{
	InitAnimation();

	// hp
	SetMaxHp(200.f);
	SetHP(200.f);

	//SetMonsterState(MonsterState::HitLight);

	// Renderer
	SpriteRenderer* sr = AddComponent<SpriteRenderer>();
	std::shared_ptr<Mesh> mesh = ResourceManager::GetInstance()->Find<Mesh>(L"RectMesh");
	std::shared_ptr<Material> material= ResourceManager::GetInstance()->Find<Material>(L"MinoMaterial");

	sr->SetMesh(mesh);
	sr->SetMaterial(material);
}

void MinosTauros::Update()
{
	Monster::Update();
}

void MinosTauros::FixedUpdate()
{
	Monster::FixedUpdate();
}

void MinosTauros::Render()
{
	Monster::Render();
}

void MinosTauros::InitAnimation()
{
	Animator* animator = AddComponent<Animator>();

	{
		std::shared_ptr<Texture2D> tex = ResourceManager::GetInstance()->
			Load<Texture2D>(L"MinoIdel", L"Monster//MinoTauros//MinoIdle.png");
		// 242 232.125
		float x = 242.f;
		float y = 232.125;
		for (int i = 0; i < 8; ++i)
		{
			wstring name = L"MinoIdle";
			name += std::to_wstring(i);

			animator->Create(name, tex,
				Vector2(0.0f, y * (float)i), Vector2(x, y), Vector2::Zero, 8, 0.1f);
		}
	}

	{
		std::shared_ptr<Texture2D> tex = ResourceManager::GetInstance()->
			Load<Texture2D>(L"MinoRun", L"Monster//MinoTauros//MinoRun.png");
		// 241.75 231.875
		float x = 241.75f;
		float y = 231.875f;
		for (int i = 0; i < 8; ++i)
		{
			wstring name = L"MinoRun";
			name += std::to_wstring(i);

			animator->Create(name, tex,
				Vector2(0.0f, y * (float)i), Vector2(x, y), Vector2::Zero, 8, 0.1f);
		}
	}

	animator->Play(L"MinoRun4");
}

void MinosTauros::Idle()
{
}

void MinosTauros::Move()
{
}

void MinosTauros::Attack()
{
}

void MinosTauros::Hit()
{
}

void MinosTauros::Dead()
{
}

void MinosTauros::HitFire()
{
}

void MinosTauros::HitFrozen()
{
}

void MinosTauros::HitLight()
{
}

