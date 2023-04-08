#include "CFrozenMisile.h"
#include "CTime.h"
#include "CAnimator.h"
#include "CSpriteRenderer.h"

#include "CResourceManager.h"

FrozenMisile::FrozenMisile()
	: Skil()
	, mKeyName(L"FrozenMisile_")
{
}

FrozenMisile::~FrozenMisile()
{
}

void FrozenMisile::Initalize()
{

	InitAnimation();
}

void FrozenMisile::Update()
{
	Skil::Update();
}

void FrozenMisile::FixedUpdate()
{
	RunMisile();
	Skil::FixedUpdate();
}

void FrozenMisile::Render()
{
	Skil::Render();
}


void FrozenMisile::InitAnimation()
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
		shared_ptr<Texture2D> tex = ResourceManager::GetInstance()->Load<Texture2D>(str, L"Frozen//" + str);

		animator->Create(str, tex, Vector2(0.0f, 0.0f), Vector2(100.f, 100.f), Vector2(0.0f, 0.0f), 6, 0.1f);
	}
}

void FrozenMisile::RunMisile()
{
	Transform* tr = GetComponent<Transform>();
	Vector3 pos = tr->GetPosition();

	mDirection.Normalize();
	pos += Vector3(mDirection.x, mDirection.y, 0.0f) * Time::GetInstance()->DeltaTime() * 3.f;

	tr->SetPosition(pos);
}
