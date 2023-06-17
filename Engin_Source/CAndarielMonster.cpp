#include "CAndarielMonster.h"
#include "CAnimator.h"
#include "CSpriteRenderer.h"
#include "CResourceManager.h"
#include "CAStar.h"
#include "CWorldManager.h"
#include "CMinoMonsterScript.h"
#include "CTime.h"

AndarielMonster::AndarielMonster()
	: Monster()
	, mSkilCoolTime(5.0f)
	, mSkilCurTime(0.0f)
{
	mSpecialCastSkil.resize(9);
}

AndarielMonster::~AndarielMonster()
{
	if (mOverlay)
		delete mOverlay;

	mOverlay = nullptr;
}

void AndarielMonster::Initalize()
{
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


	// Overlay
	mOverlay = new GameObject();
	Animator* overlayAnimator = mOverlay->AddComponent<Animator>();
	mOverlay->Paused();


	std::weak_ptr<Texture2D> tex = ResourceManager::GetInstance()->
		Load<Texture2D>(L"SpecialCast", L"Monster//Andariel//AndarielSpecialCastOverlay.png");
	int count = 4;
	// 163 108.375
	float x = 163.f;
	float y = 108.375f;
	for (int i = 0; i < 8; ++i)
	{
		wstring name = L"SpecialCast";
		name += std::to_wstring(count);

		overlayAnimator->Create(name, tex,
			Vector2(0.0f, y * (float)i), Vector2(x, y), Vector2::Zero, 18, 0.05f);

		count++;

		if (count >= 8)
			count = 0;
	}

	// SpriteRenderer
	SpriteRenderer* Oversr = mOverlay->AddComponent<SpriteRenderer>();
	std::weak_ptr<Mesh> Overmesh = ResourceManager::GetInstance()->Find<Mesh>(L"RectMesh");
	mOverlayMaterial = std::make_shared<Material>();
	mOverlayMaterial.get()->SetRenderingMode(graphics::eRenderingMode::Transparent);

	std::weak_ptr<Shader> Overshader = ResourceManager::GetInstance()->Find<Shader>(L"SpriteShader");
	mOverlayMaterial->SetShader(shader);

	Oversr->SetMesh(mesh);
	Oversr->SetMaterial(mOverlayMaterial);
}

void AndarielMonster::Update()
{
	if (GetHP() < 0)
	{
		SetMonsterState(MonsterState::Dead);
	}

	if (GetMonsterState() != MonsterState::Attack)
		mOverlay->Paused();

	mSkilCurTime += Time::GetInstance()->DeltaTime();

	Monster::Update();
	mOverlay->Update();
}

void AndarielMonster::FixedUpdate()
{
	Monster::FixedUpdate();
	mOverlay->FixedUpdate();
}

void AndarielMonster::Render()
{
	Monster::Render();
	mOverlay->Render();
}

void AndarielMonster::InitAnimation()
{
	Animator* animator = AddComponent<Animator>();
	{
		std::weak_ptr<Texture2D> tex = ResourceManager::GetInstance()->
			Load<Texture2D>(L"AndarielIdle", L"Monster//Andariel//Idle.png");

		int count = 4;
		// 173.166 179.875
		float x = 173.166f;
		float y = 179.875f;
		for (int i = 0; i < 8; ++i)
		{
			wstring name = L"AndarielIdle";
			name += std::to_wstring(count);

			animator->Create(name, tex,
				Vector2(0.0f, y * (float)i), Vector2(x, y), Vector2::Zero, 6, 0.1f);

			count++;

			if (count >= 8)
				count = 0;
		}
	}

	{
		std::weak_ptr<Texture2D> tex = ResourceManager::GetInstance()->
			Load<Texture2D>(L"AndarielRun", L"Monster//Andariel//Run.png");
		// 148 183.875
		float x = 148.f;
		float y = 183.875f;

		int count = 4;

		for (int i = 0; i < 8; ++i)
		{
			wstring name = L"AndarielRun";
			name += std::to_wstring(count);

			animator->Create(name, tex,
				Vector2(0.0f, y * (float)i), Vector2(x, y), Vector2::Zero, 12, 0.1f);

			count++;

			if (count >= 8)
				count = 0;
		}
	}

	{
		std::weak_ptr<Texture2D> tex = ResourceManager::GetInstance()->
			Load<Texture2D>(L"AndarielAttack", L"Monster//Andariel//Attack.png");
		// 247 247.75
		float x = 247.f;
		float y = 247.75f;

		int count = 4;
		for (int i = 0; i < 8; ++i)
		{
			wstring name = L"AndarielAttack";
			name += std::to_wstring(count);

			animator->Create(name, tex,
				Vector2(0.0f, y * (float)i), Vector2(x, y), Vector2::Zero, 16, 0.035f);

			count++;

			if (count >= 8)
				count = 0;
		}
	}

	{
		std::weak_ptr<Texture2D> tex = ResourceManager::GetInstance()->
			Load<Texture2D>(L"AndarielAttack2", L"Monster//Andariel//SpecialCast.png");
		// 212 181.875
		float x = 212.f;
		float y = 181.875f;

		int count = 4;
		for (int i = 0; i < 8; ++i)
		{
			wstring name = L"AndarielAttack2";
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
			Load<Texture2D>(L"AndarielDeath", L"Monster//Andariel//Death.png");
		// 203 235.625
		float x = 203.f;
		float y = 235.625f;

		int count = 4;

		for (int i = 0; i < 8; ++i)
		{
			wstring name = L"AndarielDeath";
			name += std::to_wstring(count);

			animator->Create(name, tex,
				Vector2(0.0f, y * (float)i), Vector2(x, y), Vector2::Zero, 23, 0.05f);


			count++;

			if (count >= 8)
				count = 0;
		}
	}

	animator->Play(L"AndarielIdle4");
}

void AndarielMonster::idle()
{


	Animator* animator = this->GetComponent<Animator>();
	std::wstring& name = animator->GetPlayAnimation()->AnimationName();

	wstring playName = L"AndarielIdle";
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

void AndarielMonster::move()
{
	Animator* animator = GetComponent<Animator>();
	std::wstring& name = animator->GetPlayAnimation()->AnimationName();

	wstring playName = L"AndarielRun";
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

void AndarielMonster::attack()
{
	Animator* animator = GetComponent<Animator>();
	std::wstring& name = animator->GetPlayAnimation()->AnimationName();

	wstring playName = L"AndarielAttack";
	UINT Index = GetIndex();
	playName += std::to_wstring(Index);


	if (name.find(L"Attack") == wstring::npos)
	{
		if (mSkilCurTime >= mSkilCoolTime)
		{
			mSkilCurTime = 0.0f;

			wstring specialCast = L"AndarielAttack2";
			UINT Index = GetIndex();
			specialCast += std::to_wstring(Index);

			animator->Play(specialCast, false);

			//Overlay
			Transform* Tr = GetComponent<Transform>();
			Vector3 Pos = Tr->GetPosition();

			mOverlay->Active();
			Transform* OverlayTr = mOverlay->GetComponent<Transform>();
			OverlayTr->SetPosition(Pos);
			OverlayTr->SetSize(Vector3(400.f, 400.f, 1.0f));

			wstring cast = L"SpecialCast";
			cast += std::to_wstring(Index);

			mOverlay->GetComponent<Animator>()->Play(cast, false);

			return;
		}
		
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

void AndarielMonster::hit()
{

}

void AndarielMonster::monsterDead()
{
	Animator* animator = GetComponent<Animator>();
	std::wstring& name = animator->GetPlayAnimation()->AnimationName();

	wstring playName = L"AndarielDeath";
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

void AndarielMonster::hitFire()
{

}

void AndarielMonster::hitFrozen()
{
}

void AndarielMonster::hitLight()
{
}

