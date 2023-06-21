#include "CDiabloMonster.h"
#include "CAnimator.h"
#include "CSpriteRenderer.h"
#include "CResourceManager.h"
#include "CAStar.h"
#include "CWorldManager.h"
#include "CTime.h"
#include "CDiabloSkilBreath.h"
#include "CObject.h"
#include "CDiabloScript.h"
#include "CDiabloSkilFireStom.h"

#include <random>


extern std::random_device random;
extern std::mt19937 gen;
std::uniform_int_distribution<int> skil(1, 1000);


DiabloMonster::DiabloMonster()
	: Monster()
	, mSkilCoolTime(8.0f)
	, mSkilCurTime(0.0f)
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
	SetMaxHp(700.f);
	SetHP(700.f);
	SetDamege(55.f);

	//SetMonsterStatusEffect(eElementType::HitFrozen);

	// Astar
	AddComponent<AStar>();

	// Script
	AddComponent<DiabloScript>();

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
	SetMonsterStatusEffect(eElementType::None);

	mSkilCurTime += Time::GetInstance()->DeltaTime();

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
				Vector2(0.0f, y * (float)i), Vector2(x, y), Vector2::Zero, Vector2(250.f, 250.f),18, 0.15f);

			animator->GetEvent(name, 11) = std::bind(&DiabloMonster::DiabloSpecialCast1, this);

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

			animator->GetEvent(name, 8) = std::bind(&DiabloMonster::DiabloSpecialCast2, this);

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

void DiabloMonster::CreateNextScenePotal()
{
}

void DiabloMonster::DiabloSpecialCast1()
{
	DiabloSkilFireStom* skil = Object::Instantiate<DiabloSkilFireStom>(eLayerType::MonsterSkil, true);
	Transform* skilTr = skil->GetComponent<Transform>();

	Transform* monsterTr = GetComponent<Transform>();
	skilTr->SetPosition(monsterTr->GetPosition());
}

void DiabloMonster::DiabloSpecialCast2()
{
	//Overlay
	Transform* Tr = GetComponent<Transform>();
	Vector3 Pos = Tr->GetPosition();

	GameObject* player = WorldManager::GetInstance()->GetPlayer();
	Transform* playerTr = player->GetComponent<Transform>();
	Vector3 playerPos = playerTr->GetPosition();


	Vector2 vec = playerPos - Vector2(Pos.x, Pos.y);
	Vector2 Vec1 = Vector2(0.0f, 0.0f);

	if (vec.x <= 0.f)
		Vec1 = Vector2(-1.0f, 0.0f);
	else
		Vec1 = Vector2(1.0f, 0.0f);

	Vec1.Normalize();
	vec.Normalize();

	float that = Vec1.Dot(vec);
	float radian = acos(that);

	float radius = 200.f;

	// 원의 방정식
	// https://nenara.com/68
	int x = cosf(radian) * radius;
	int y = sinf(radian) * radius;

	if (vec.x < 0)
		x *= -1.f;
	if (vec.y < 0)
		y *= -1.f;

	Vector3 SearchPos = Pos;
	SearchPos.x += x;
	SearchPos.y += y;


	DiabloSkilBreath* skil = Object::Instantiate<DiabloSkilBreath>(eLayerType::MonsterSkil, true);
	Transform* skilTr = skil->GetComponent<Transform>();

	skilTr->SetPosition(SearchPos);
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
		if (mSkilCurTime >= mSkilCoolTime)
		{

			mSkilCurTime = 0.0f;

			wstring skilname = L"";
			if (skil(gen) % 2)
			{
				wstring specialCast = L"DiabloAttack2";
				specialCast += std::to_wstring(Index);

				skilname = specialCast;
			}
			else
			{
				wstring specialCast = L"DiabloAttack3";
				specialCast += std::to_wstring(Index);

				skilname = specialCast;
			}

			if(skilname != L"")
				animator->Play(skilname, false);
		}
		else
		{
			animator->Play(playName, false);
		}

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