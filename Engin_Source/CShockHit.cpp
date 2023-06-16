#include "CShockHit.h"
#include "CAnimator.h"
#include "CResourceManager.h"
#include "CTransform.h"
#include "CSpriteRenderer.h"
#include "CObject.h"
#include "CLight.h"
#include "CTime.h"

ShockHit::ShockHit()
	: Skil()
	, mDeathTime(0.0f)
{
	SetElementType(eElementType::HitLight);
}

ShockHit::~ShockHit()
{
}

void ShockHit::Initalize()
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
	weak_ptr<Shader> shader= ResourceManager::GetInstance()->Find<Shader>(L"SpriteShader");
	mMaterial.get()->SetShader(shader);

	weak_ptr<Texture2D> tex = ResourceManager::GetInstance()->Find<Texture2D>(L"ShockHit");
	mMaterial->SetTexture(eTextureSlot::T0, tex);
	mMaterial.get()->SetRenderingMode(graphics::eRenderingMode::Transparent);

	sr->SetMesh(mesh);
	sr->SetMaterial(mMaterial);
}

void ShockHit::Update()
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

	Vector3 TargetPos = mTargetTr->GetPosition();
	GetComponent<Transform>()->SetPosition(TargetPos);

	Skil::Update();
}

void ShockHit::InitAnimation()
{
	Animator* animator = AddComponent<Animator>();

	weak_ptr<Texture2D> tex =
		ResourceManager::GetInstance()->Load<Texture2D>(L"ShockHit", L"LightBolt//ShockHit.png");

	float drrationTime = 0.02f;
	int spriteLenght = 20.f;

	animator->Create(L"ShockHit", tex, Vector2::Zero, Vector2(113.f, 99.f), Vector2::Zero, spriteLenght, drrationTime);
	animator->Play(L"ShockHit");

	mDeathTime = spriteLenght * drrationTime;
}

void ShockHit::SetTarget(GameObject* obj)
{
	mTargetTr = obj->GetComponent<Transform>();
}
