#include "CMephistoMonster.h"
#include "CAnimator.h"
#include "CSpriteRenderer.h"
#include "CResourceManager.h"
#include "CAStar.h"
#include "CWorldManager.h"
#include "CMinoMonsterScript.h"


MephistoMonster::MephistoMonster()
	: Monster()
{
}

MephistoMonster::~MephistoMonster()
{
}

void MephistoMonster::Initalize()
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

void MephistoMonster::Update()
{
	if (GetHP() < 0)
	{
		SetMonsterState(MonsterState::Dead);
	}

	Monster::Update();
}

void MephistoMonster::FixedUpdate()
{
	Monster::FixedUpdate();
}

void MephistoMonster::Render()
{
	Monster::Render();
}

void MephistoMonster::InitAnimation()
{
	Animator* animator = AddComponent<Animator>();
	{
		std::weak_ptr<Texture2D> tex = ResourceManager::GetInstance()->
			Load<Texture2D>(L"MephistoIdle", L"Monster//Mephisto//idle.png");
		// 307.076 307
		float x = 307.076f;
		float y = 307.f;

		int count = 4;

		for (int i = 0; i < 8; ++i)
		{
			wstring name = L"MephistoIdle";
			name += std::to_wstring(count);

			animator->Create(name, tex,
				Vector2(0.0f, y * (float)i), Vector2(x, y), Vector2::Zero, 13, 0.1f);

			count++;

			if (count >= 8)
				count = 0;
		}

	}

	{
		std::weak_ptr<Texture2D> tex = ResourceManager::GetInstance()->
			Load<Texture2D>(L"MephistoRun", L"Monster//Mephisto//Run.png");
		// 307.0769 307
		float x = 307.0769f;
		float y = 307.f;

		int count = 4;
		for (int i = 0; i < 8; ++i)
		{
			wstring name = L"MephistoRun";
			name += std::to_wstring(count);

			animator->Create(name, tex,
				Vector2(0.0f, y * (float)i), Vector2(x, y), Vector2::Zero, 13, 0.1f);

			count++;

			if (count >= 8)
				count = 0;
		}
	}

	{
		std::weak_ptr<Texture2D> tex = ResourceManager::GetInstance()->
			Load<Texture2D>(L"MephistoAttack", L"Monster//Mephisto//Attack.png");
		// 202.0625 202.125
		float x = 307.f;
		float y = 307.f;

		int count = 4;
		for (int i = 0; i < 8; ++i)
		{
			wstring name = L"MephistoAttack";
			name += std::to_wstring(count);

			animator->Create(name, tex,
				Vector2(0.0f, y * (float)i), Vector2(x, y), Vector2::Zero, 18, 0.05f);

			count++;

			if (count >= 8)
				count = 0;
		}
	}

	{
		std::weak_ptr<Texture2D> tex = ResourceManager::GetInstance()->
			Load<Texture2D>(L"MephistoDeath", L"Monster//Mephisto//Death.png");
		// 202.05 202.125
		float x = 307.f;
		float y = 307.f;

		int count = 4;

		for (int i = 0; i < 8; ++i)
		{
			wstring name = L"MephistoDeath";
			name += std::to_wstring(count);

			animator->Create(name, tex,
				Vector2(0.0f, y * (float)i), Vector2(x, y), Vector2::Zero, 25, 0.1f);

			count++;

			if (count >= 8)
				count = 0;
		}
	}

	animator->Play(L"MephistoDeath4");
}

void MephistoMonster::idle()
{


	Animator* animator = this->GetComponent<Animator>();
	std::wstring& name = animator->GetPlayAnimation()->AnimationName();

	wstring playName = L"MephistoIdle";
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

void MephistoMonster::move()
{
	Animator* animator = GetComponent<Animator>();
	std::wstring& name = animator->GetPlayAnimation()->AnimationName();

	wstring playName = L"MephistoRun";
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

void MephistoMonster::attack()
{
	Animator* animator = GetComponent<Animator>();
	std::wstring& name = animator->GetPlayAnimation()->AnimationName();

	wstring playName = L"MephistoAttack";
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

void MephistoMonster::hit()
{

}

void MephistoMonster::monsterDead()
{
	Animator* animator = GetComponent<Animator>();
	std::wstring& name = animator->GetPlayAnimation()->AnimationName();

	wstring playName = L"MephistoDeath";
	UINT index = GetDirection();
	playName += std::to_wstring(index);

	if (name.find(L"Death") != wstring::npos &&
		animator->GetPlayAnimation()->IsComplete())
	{
		this->Death();
	}

	if (playName == name)
		return;

	animator->Play(playName, false);
}

void MephistoMonster::hitFire()
{

}

void MephistoMonster::hitFrozen()
{
}

void MephistoMonster::hitLight()
{
}

