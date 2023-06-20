#include "CDiabloMonster.h"
#include "CAnimator.h"
#include "CSpriteRenderer.h"
#include "CResourceManager.h"
#include "CAStar.h"
#include "CWorldManager.h"
#include "CMinoMonsterScript.h"


DiabloMonster::DiabloMonster()
	: Monster()
{
}

DiabloMonster::~DiabloMonster()
{
}

void DiabloMonster::Initalize()
{
	Monster::Initalize();

	InitAnimation();

	Transform* minoTr = GetComponent<Transform>();
	minoTr->SetSize(Vector3(500.f, 500.f, 1.0f));

	// hp
	SetMaxHp(1000.f);
	SetHP(1000.f);
	SetDamege(55.f);

	//SetMonsterStatusEffect(eElementType::HitFrozen);

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

void DiabloMonster::Update()
{
	Monster::Update();

	Transform* shadowTr = mShadow->GetComponent<Transform>();
	Transform* OwnerTr = GetComponent<Transform>();

	Vector3 OwnerPos = OwnerTr->GetPosition();
	Vector3 OwnerSize = OwnerTr->GetSize();

	float shadowY = OwnerPos.y - (OwnerSize.y * 0.25f);

	Vector3 ShadowPos = shadowTr->GetPosition();
	ShadowPos = OwnerPos;
	ShadowPos.y = shadowY;

	shadowTr->SetPosition(ShadowPos);
	OwnerSize.x = OwnerSize.x * 0.15f;
	OwnerSize.y = OwnerSize.y * 0.075f;
	shadowTr->SetSize(OwnerSize);
}

void DiabloMonster::FixedUpdate()
{
	Monster::FixedUpdate();
}

void DiabloMonster::Render()
{
	Monster::Render();
}

void DiabloMonster::InitAnimation()
{
	Animator* animator = AddComponent<Animator>();
	{
		int count = 4;
		std::weak_ptr<Texture2D> tex = ResourceManager::GetInstance()->
			Load<Texture2D>(L"DiabloIdle", L"Monster//Diablo//DiabloIdle.png");
		// 263 212
		float x = 263.f;
		float y = 212.f;
		for (int i = 0; i < 8; ++i)
		{
			wstring name = L"DiabloIdle";
			name += std::to_wstring(count);

			animator->Create(name, tex,
				Vector2(0.0f, y * (float)i), Vector2(x, y), Vector2::Zero, Vector2(250.f, 250.f), 12, 0.1f);

			count++;

			if (count >= 8)
				count = 0;
		}

	}

	{
		int count = 4;
		std::weak_ptr<Texture2D> tex = ResourceManager::GetInstance()->
			Load<Texture2D>(L"DiabloRun", L"Monster//Diablo//Wlak.png");
		// 293 187.5
		float x = 293.f;
		float y = 187.5f;
		for (int i = 0; i < 8; ++i)
		{
			wstring name = L"DiabloRun";
			name += std::to_wstring(count);

			animator->Create(name, tex,
				Vector2(0.0f, y * (float)i), Vector2(x, y), Vector2::Zero, Vector2(250.f, 250.f), 12, 0.1f);

			count++;

			if (count >= 8)
				count = 0;
		}
	}

	{
		int count = 4;
		std::weak_ptr<Texture2D> tex = ResourceManager::GetInstance()->
			Load<Texture2D>(L"DiabloAttack", L"Monster//Diablo//Attack.png");
		// 288.05 286.875
		float x = 288.05f;
		float y = 286.875f;
		for (int i = 0; i < 8; ++i)
		{
			wstring name = L"DiabloAttack";
			name += std::to_wstring(count);

			animator->Create(name, tex,
				Vector2(0.0f, y * (float)i), Vector2(x, y), Vector2::Zero, Vector2(250.f, 250.f),20, 0.05f);

			count++;

			if (count >= 8)
				count = 0;
		}
	}

	{
		int count = 4;
		std::weak_ptr<Texture2D> tex = ResourceManager::GetInstance()->
			Load<Texture2D>(L"DiabloAttack2", L"Monster//Diablo//SpecialCast1.png");
		// 277 203.875
		float x = 277.f;
		float y = 203.875f;
		for (int i = 0; i < 8; ++i)
		{
			wstring name = L"DiabloAttack2";
			name += std::to_wstring(count);

			animator->Create(name, tex,
				Vector2(0.0f, y * (float)i), Vector2(x, y), Vector2::Zero, Vector2(250.f, 250.f),18, 0.1f);

			count++;

			if (count >= 8)
				count = 0;
		}
	}

	{
		int count = 4;
		std::weak_ptr<Texture2D> tex = ResourceManager::GetInstance()->
			Load<Texture2D>(L"DiabloAttack3", L"Monster//Diablo//SpecialCast2.png");
		// 266 256.875
		float x = 266.f;
		float y = 256.875f;
		for (int i = 0; i < 8; ++i)
		{
			wstring name = L"DiabloAttack3";
			name += std::to_wstring(count);

			animator->Create(name, tex,
				Vector2(0.0f, y * (float)i), Vector2(x, y), Vector2::Zero, Vector2(200.f, 200.f), 17, 0.1f);

			count++;

			if (count >= 8)
				count = 0;
		}
	}

	{
		int count = 4;
		std::weak_ptr<Texture2D> tex = ResourceManager::GetInstance()->
			Load<Texture2D>(L"DiabloDeath", L"Monster//Diablo//Death.png");
		// 307.266 205.625
		float x = 307.266f;
		float y = 205.625f;
		for (int i = 0; i < 8; ++i)
		{
			wstring name = L"DiabloDeath";
			name += std::to_wstring(count);

			animator->Create(name, tex,
				Vector2(0.0f, y * (float)i), Vector2(x, y), Vector2::Zero, Vector2(200.f, 200.f), 15, 0.1f);

			count++;

			if (count >= 8)
				count = 0;
		}
	}

	animator->Play(L"DiabloIdle4");
}

void DiabloMonster::idle()
{


	Animator* animator = this->GetComponent<Animator>();
	std::wstring& name = animator->GetPlayAnimation()->AnimationName();

	wstring playName = L"DiabloIdle";
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

void DiabloMonster::move()
{
	Animator* animator = GetComponent<Animator>();
	std::wstring& name = animator->GetPlayAnimation()->AnimationName();

	wstring playName = L"DiabloRun";
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

void DiabloMonster::attack()
{
	Animator* animator = GetComponent<Animator>();
	std::wstring& name = animator->GetPlayAnimation()->AnimationName();

	wstring playName = L"DiabloAttack";
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

void DiabloMonster::hit()
{
}

void DiabloMonster::monsterDead()
{
	Animator* animator = GetComponent<Animator>();
	std::wstring& name = animator->GetPlayAnimation()->AnimationName();

	wstring playName = L"DiabloDeath";
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

void DiabloMonster::hitFire()
{

}

void DiabloMonster::hitFrozen()
{
}

void DiabloMonster::hitLight()
{
}

