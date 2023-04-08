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

	// ·»´õ·¯
	SpriteRenderer* sr = AddComponent<SpriteRenderer>();
	shared_ptr<Mesh> mesh = ResourceManager::GetInstance()->Find<Mesh>(L"RectMesh");
	shared_ptr<Material> material = ResourceManager::GetInstance()->Find<Material>(L"FrozenMisileMaterial");

	shared_ptr<Texture2D> tex = ResourceManager::GetInstance()->Find<Texture2D>(L"FrozenMisile_00");
	material->SetTexture(eTextureSlot::T0, tex);

	sr->SetMesh(mesh);
	sr->SetMaterial(material);
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
		shared_ptr<Texture2D> tex = ResourceManager::GetInstance()->Load<Texture2D>(str, L"Frozen//" + str + L".png");

		animator->Create(str, tex, Vector2(0.0f, 0.0f), Vector2(100.f, 100.f), Vector2(0.0f, 0.0f), 6, 0.1f);
	}
	animator->Play(L"FrozenMisile_00");
}

void FrozenMisile::RunMisile()
{
	Transform* tr = GetComponent<Transform>();
	Vector3 pos = tr->GetPosition();

	pos += Vector3(1.0f, 0.0f, 0.0f) * Time::GetInstance()->DeltaTime() * 2.f;

	cout << pos.x << "         " << pos.y << endl;
	tr->SetPosition(pos);
}
