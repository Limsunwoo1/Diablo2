#include "CRadamentMonster.h"
#include "CAnimator.h"
#include "CSpriteRenderer.h"
#include "CResourceManager.h"
#include "CAStar.h"
#include "CWorldManager.h"
#include "CMinoMonsterScript.h"


RadamentMonster::RadamentMonster()
	: Monster()
{
}

RadamentMonster::~RadamentMonster()
{
}

void RadamentMonster::Initalize()
{
	Monster::Initalize();

	InitAnimation();

	Transform* minoTr = GetComponent<Transform>();
	minoTr->SetSize(Vector3(350.f, 350.f, 1.0f));

	// hp
	SetMaxHp(200.f);
	SetHP(200.f);

	//SetMonsterStatusEffect(MonsterStatusEffect::HitFrozen);

	// Astar
	AddComponent<AStar>();

	// Script
	AddComponent<MinoMonsterScript>();

	// Renderer
	SpriteRenderer* sr = AddComponent<SpriteRenderer>();
	std::weak_ptr<Mesh> mesh = ResourceManager::GetInstance()->Find<Mesh>(L"RectMesh");
	mMaterial = std::make_shared<Material>();
	mMaterial.get()->SetRenderingMode(graphics::eRenderingMode::Transparent);

	std::weak_ptr<Shader> shader = ResourceManager::GetInstance()->Find<Shader>(L"SpriteShader");
	mMaterial->SetShader(shader);

	sr->SetMesh(mesh);
	sr->SetMaterial(mMaterial);
}

void RadamentMonster::Update()
{


	Monster::Update();
}

void RadamentMonster::FixedUpdate()
{
	Monster::FixedUpdate();
}

void RadamentMonster::Render()
{
	Monster::Render();
}

void RadamentMonster::InitAnimation()
{
	Animator* animator = AddComponent<Animator>();
	{
		std::weak_ptr<Texture2D> tex = ResourceManager::GetInstance()->
			Load<Texture2D>(L"RadamentIdle", L"Monster//Radament//idle.png");
		// 202 202.125
		float x = 202.f;
		float y = 202.125f;
		for (int i = 0; i < 8; ++i)
		{
			wstring name = L"RadamentIdle";
			name += std::to_wstring(i);

			animator->Create(name, tex,
				Vector2(0.0f, y * (float)i), Vector2(x, y), Vector2::Zero, 12, 0.1f);
		}

	}

	{
		std::weak_ptr<Texture2D> tex = ResourceManager::GetInstance()->
			Load<Texture2D>(L"RadamentRun", L"Monster//Radament//Run.png");
		// 202.055 202.125
		float x = 202.055f;
		float y = 202.125f;
		for (int i = 0; i < 8; ++i)
		{
			wstring name = L"RadamentRun";
			name += std::to_wstring(i);

			animator->Create(name, tex,
				Vector2(0.0f, y * (float)i), Vector2(x, y), Vector2::Zero, 18, 0.1f);
		}
	}

	{
		std::weak_ptr<Texture2D> tex = ResourceManager::GetInstance()->
			Load<Texture2D>(L"RadamentAttack", L"Monster//Radament//Attack.png");
		// 202.0625 202.125
		float x = 202.0625f;
		float y = 202.125f;
		for (int i = 0; i < 8; ++i)
		{
			wstring name = L"RadamentAttack";
			name += std::to_wstring(i);

			animator->Create(name, tex,
				Vector2(0.0f, y * (float)i), Vector2(x, y), Vector2::Zero, 16, 0.05f);

			animator->GetEvent(name, 10) = std::bind(&Monster::Attack, this);
		}
	}

	{
		std::weak_ptr<Texture2D> tex = ResourceManager::GetInstance()->
			Load<Texture2D>(L"RadamentDeath", L"Monster//Radament//Death.png");
		// 202.05 202.125
		float x = 202.05f;
		float y = 202.125f;
		for (int i = 0; i < 8; ++i)
		{
			wstring name = L"RadamentDeath";
			name += std::to_wstring(i);

			animator->Create(name, tex,
				Vector2(0.0f, y * (float)i), Vector2(x, y), Vector2::Zero, 20, 0.1f);
		}
	}

	animator->Play(L"RadamentIdle4");
}

void RadamentMonster::idle()
{


	Animator* animator = this->GetComponent<Animator>();
	std::wstring& name = animator->GetPlayAnimation()->AnimationName();

	wstring playName = L"RadamentIdle";
	UINT index = GetDirection();
	playName += std::to_wstring(index);

	if (name.find(L"Attack") != std::wstring::npos)
	{
		if (!animator->GetPlayAnimation()->IsComplete())
		{
			SetMonsterState(MonsterState::Attack);
			return;
		}
	}

	if (playName == name)
		return;

	animator->Play(playName);
}

void RadamentMonster::move()
{
	Animator* animator = GetComponent<Animator>();
	std::wstring& name = animator->GetPlayAnimation()->AnimationName();

	wstring playName = L"RadamentRun";
	UINT index = GetDirection();
	playName += std::to_wstring(index);

	if (name.find(L"Attack") != std::wstring::npos)
	{
		if (!animator->GetPlayAnimation()->IsComplete())
		{
			SetMonsterState(MonsterState::Attack);
			return;
		}
	}

	if (playName == name)
		return;

	animator->Play(playName);

}

void RadamentMonster::attack()
{
	Animator* animator = GetComponent<Animator>();
	std::wstring& name = animator->GetPlayAnimation()->AnimationName();

	wstring playName = L"RadamentAttack";
	UINT Index = GetIndex();
	playName += std::to_wstring(Index);


	if (name.find(L"Attack") == wstring::npos)
	{
		animator->Play(playName, false);
	}
	else
	{
		if (animator->GetPlayAnimation()->IsComplete())
		{
			SetMonsterState(MonsterState::Idle);
		}
	}
}

void RadamentMonster::hit()
{

}

void RadamentMonster::monsterDead()
{
	Animator* animator = GetComponent<Animator>();
	std::wstring& name = animator->GetPlayAnimation()->AnimationName();

	wstring playName = L"RadamentDeath";
	UINT index = GetDirection();
	playName += std::to_wstring(index);

	if (name.find(L"Death") != wstring::npos &&
		animator->GetPlayAnimation()->IsComplete())
	{
		mShadow->Paused();
		this->Paused();
	}

	if (playName == name)
		return;

	animator->Play(playName, false);
}

void RadamentMonster::hitFire()
{

}

void RadamentMonster::hitFrozen()
{
}

void RadamentMonster::hitLight()
{
}

