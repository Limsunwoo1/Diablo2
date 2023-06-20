#include "CFrozenArmer.h"
#include "CAnimator.h"
#include "CResourceManager.h"
#include "CTransform.h"
#include "CSpriteRenderer.h"
#include "CObject.h"
#include "CLight.h"
#include "CTime.h"
#include "CWorldManager.h"
#include "Cplayer.h"

FrozenArmer::FrozenArmer()
	: Skil()
	, mDeathTime(8.0f)
{
	SetElementType(eElementType::None);
}

FrozenArmer::~FrozenArmer()
{
}

void FrozenArmer::Initalize()
{
	// 사이즈
	Transform* tr = GetComponent<Transform>();
	tr->SetSize(Vector3(300.f, 300.f, 1.0f));

	// 애니메이터 초기화
	InitAnimation();

	// 렌더러
	SpriteRenderer* sr = AddComponent<SpriteRenderer>();
	weak_ptr<Mesh> mesh = ResourceManager::GetInstance()->Find<Mesh>(L"RectMesh");

	mMaterial = std::make_shared<Material>();
	weak_ptr<Shader> shader = ResourceManager::GetInstance()->Find<Shader>(L"SpriteShader");
	mMaterial.get()->SetShader(shader);

	weak_ptr<Texture2D> tex = ResourceManager::GetInstance()->Find<Texture2D>(L"FrozenArmer");
	mMaterial->SetTexture(eTextureSlot::T0, tex);
	mMaterial.get()->SetRenderingMode(graphics::eRenderingMode::Transparent);

	sr->SetMesh(mesh);
	sr->SetMaterial(mMaterial);
}

void FrozenArmer::Update()
{
	if (mDeathTime < 0.f)
	{
		Death();
		return;
	}
	mDeathTime -= Time::GetInstance()->DeltaTime();

	if (mTargetTr == nullptr)
		return;

	if (mTargetTr->GetOwner()->GetState() == eState::dead)
		return;

	Player* player = dynamic_cast<Player*>(WorldManager::GetInstance()->GetPlayer());
	float hp = player->GetHP();
	float mp = player->GetMP();

	float MaxHp = player->GetMaxHP();
	float Maxmp = player->GetMaxMP();

	hp += Time::GetInstance()->DeltaTime() * 12;
	mp += Time::GetInstance()->DeltaTime() * 14.f;

	if (hp > MaxHp)
		hp = MaxHp;
	if (mp > Maxmp)
		mp = Maxmp;

	player->SetHP(hp);
	player->SetMP(mp);

	Vector3 TargetPos = mTargetTr->GetPosition();
	Vector3 TargetSize = mTargetTr->GetSize();
	TargetPos.y += TargetSize.y * 0.1f;

	GetComponent<Transform>()->SetPosition(TargetPos);

	Skil::Update();
}

void FrozenArmer::InitAnimation()
{
	Animator* animator = AddComponent<Animator>();

	weak_ptr<Texture2D> tex =
		ResourceManager::GetInstance()->Load<Texture2D>(L"FrozenArmer", L"FrozenArmer//FrozenArmer.png");

	animator->Create(L"FrozenArmer", tex, Vector2::Zero, Vector2(184.f, 140.f), Vector2::Zero, 12, 0.1f);
	animator->Play(L"FrozenArmer");
}

void FrozenArmer::SetTarget(GameObject* obj)
{
	mTargetTr = obj->GetComponent<Transform>();
}
