#include "CFrozenMissile.h"
#include "CTime.h"
#include "CAnimator.h"
#include "CSpriteRenderer.h"

#include "CResourceManager.h"

FrozenMissile::FrozenMissile()
	: Skil()
	, mKeyName(L"FrozenMisile_")
	, mTime(0.0f)
	, mbMode(eMisileMode::Straight)
	, mOrb(nullptr)
{
}

FrozenMissile::~FrozenMissile()
{
}

void FrozenMissile::Initalize()
{
	InitAnimation();

	// 렌더러
	SpriteRenderer* sr = AddComponent<SpriteRenderer>();
	weak_ptr<Mesh> mesh = ResourceManager::GetInstance()->Find<Mesh>(L"RectMesh");
	weak_ptr<Material> material = ResourceManager::GetInstance()->Find<Material>(L"FrozenMisileMaterial");

	weak_ptr<Texture2D> tex = ResourceManager::GetInstance()->Find<Texture2D>(L"FrozenMisile_00");
	material.lock()->SetTexture(eTextureSlot::T0, tex);

	sr->SetMesh(mesh);
	sr->SetMaterial(material);

	// 사이즈
	Transform* tr = GetComponent<Transform>();
	tr->SetSize(Vector3(200.f, 200.f, 1.f));

	Death();
}

void FrozenMissile::Update()
{
	mTime += Time::GetInstance()->DeltaTime();

	Skil::Update();
}

void FrozenMissile::FixedUpdate()
{
	RunMisile();
	Skil::FixedUpdate();
}

void FrozenMissile::Render()
{
	Skil::Render();
}

void FrozenMissile::InitAnimation()
{
	Animator* animator = AddComponent<Animator>();

	for (int i = 0; i < 16; ++i)
	{
		wstring str = mKeyName;

		if (i < 10)
		{
			str += L"0" + to_wstring(i);
		}
		else
		{
			str += to_wstring(i);
		}
		weak_ptr<Texture2D> tex = ResourceManager::GetInstance()->Load<Texture2D>(str, L"Frozen//" + str + L".png");

		animator->Create(str, tex, Vector2(0.0f, 0.0f), Vector2(100.f, 100.f), Vector2(0.0f, 0.0f), 6, 0.1f);
	}
	animator->Play(L"FrozenMisile_12");
}

void FrozenMissile::RunMisile()
{
	Transform* tr = GetComponent<Transform>();
	Vector3 pos = tr->GetPosition();
	Vector3 rotation = tr->GetRotation();

	switch (mbMode)
	{
	case eMisileMode::Straight:
	{

	}
		break;
	case eMisileMode::Rotation:
	{
		if (mTime >= 1.0f)
			mbMode = eMisileMode::Straight;

		rotation.z += Time::GetInstance()->DeltaTime() * 60.0f;
	}
		break;
	case eMisileMode::ReversRotation:
	{
		if (mTime >= 1.0f)
			mbMode = eMisileMode::Straight;

		rotation.z -= Time::GetInstance()->DeltaTime() * 60.0f;
	}
		break;
	default:
		break;
	}
	
	pos += tr->Right() * Time::GetInstance()->DeltaTime() * 400.0f;
	tr->SetPosition(pos);
	tr->SetRotation(rotation);
}
