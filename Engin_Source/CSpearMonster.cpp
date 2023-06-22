#include "CSpearMonster.h"
#include "CAnimator.h"
#include "CSpriteRenderer.h"
#include "CResourceManager.h"
#include "CAStar.h"
#include "CWorldManager.h"
#include "CMinoMonsterScript.h"


SpearMonster::SpearMonster()
	: Monster()
{
}

SpearMonster::~SpearMonster()
{
}

void SpearMonster::Initalize()
{
	Monster::Initalize();

	InitAnimation();

	Transform* minoTr = GetComponent<Transform>();
	minoTr->SetSize(Vector3(350.f, 350.f, 1.0f));

	// hp
	SetMaxHp(40.f);
	SetHP(40.f);

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

	std::weak_ptr<Shader> shader= ResourceManager::GetInstance()->Find<Shader>(L"SpriteShader");
	mMaterial->SetShader(shader);

	sr->SetMesh(mesh);
	sr->SetMaterial(mMaterial);
}

void SpearMonster::Update()
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

void SpearMonster::FixedUpdate()
{
	Monster::FixedUpdate();
}

void SpearMonster::Render()
{
	Monster::Render();
}

void SpearMonster::InitAnimation()
{
	Animator* animator = AddComponent<Animator>();
	{
		std::weak_ptr<Texture2D> tex = ResourceManager::GetInstance()->
			Load<Texture2D>(L"SpearIdle", L"Monster//SpearMonster//Idle.png");
		// 128.7 100.875
		float x = 128.7f;
		float y = 100.875f;
		for (int i = 0; i < 8; ++i)
		{
			wstring name = L"SpearIdle";
			name += std::to_wstring(i);

			animator->Create(name, tex,
				Vector2(0.0f, y * (float)i), Vector2(x, y), Vector2::Zero, 10, 0.1f);
		}

	}

	{
		std::weak_ptr<Texture2D> tex = ResourceManager::GetInstance()->
			Load<Texture2D>(L"SpearRun", L"Monster//SpearMonster//Run.png");
		// 125.75 101
		float x = 125.75f;
		float y = 101.f;
		for (int i = 0; i < 8; ++i)
		{
			wstring name = L"SpearRun";
			name += std::to_wstring(i);

			animator->Create(name, tex,
				Vector2(0.0f, y * (float)i), Vector2(x, y), Vector2::Zero, 8, 0.1f);
		}
	}

	{
		std::weak_ptr<Texture2D> tex = ResourceManager::GetInstance()->
			Load<Texture2D>(L"SpearAttack", L"Monster//SpearMonster//Attack.png");
		// 164.95 167.875
		float x = 164.95f;
		float y = 167.875f;
		for (int i = 0; i < 8; ++i)
		{
			wstring name = L"SpearAttack";
			name += std::to_wstring(i);

			animator->Create(name, tex,
				Vector2(0.0f, y * (float)i), Vector2(x, y), Vector2::Zero, 20, 0.05f);

			animator->GetEvent(name, 13) = std::bind(&Monster::Attack, this);
		}
	}

	{
		std::weak_ptr<Texture2D> tex = ResourceManager::GetInstance()->
			Load<Texture2D>(L"SpearDeath", L"Monster//SpearMonster//Death.png");
		// 161.0526 149
		float x = 161.0526f;
		float y = 149.f;
		for (int i = 0; i < 8; ++i)
		{
			wstring name = L"SpearDeath";
			name += std::to_wstring(i);

			animator->Create(name, tex,
				Vector2(0.0f, y * (float)i), Vector2(x, y), Vector2::Zero, 15, 0.1f);

			animator->GetCompleteEvent(name) = std::bind(&Monster::DropItem, this);

			//animator->GetEvent(name, 14) = std::bind(&Monster::DropItem, this);
		}
	}

	animator->Play(L"SpearIdle4");
}

void SpearMonster::idle()
{


	Animator* animator = this->GetComponent<Animator>();
	std::wstring& name = animator->GetPlayAnimation()->AnimationName();

	wstring playName = L"SpearIdle";
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

void SpearMonster::move()
{
	Animator* animator = GetComponent<Animator>();
	std::wstring& name = animator->GetPlayAnimation()->AnimationName();

	wstring playName = L"SpearRun";
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

void SpearMonster::attack()
{
	Animator* animator = GetComponent<Animator>();
	std::wstring& name = animator->GetPlayAnimation()->AnimationName();

	wstring playName = L"SpearAttack";
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

void SpearMonster::hit()
{
}

void SpearMonster::monsterDead()
{
	Animator* animator = GetComponent<Animator>();
	std::wstring& name = animator->GetPlayAnimation()->AnimationName();

	wstring playName = L"SpearDeath";
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

void SpearMonster::hitFire()
{

}

void SpearMonster::hitFrozen()
{
}

void SpearMonster::hitLight()
{
}

