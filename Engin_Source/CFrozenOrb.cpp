#include "CFrozenOrb.h"
#include "CFrozenMisile.h"
#include "CAnimator.h"
#include "CSpriteRenderer.h"
#include "CGenericAnimator.h"

#include "CTime.h"
#include "CResourceManager.h"

FrozenOrb::FrozenOrb()
	: Skil()
	, mFrozenMisile{}
{
}

FrozenOrb::~FrozenOrb()
{
}

void FrozenOrb::Initalize()
{
	for (int i = 0; i < 20; ++i)
		mFrozenMisile.emplace_back(new FrozenMisile);

	// ↗ ↙ ↘ ↖ → ←
	mMisileDirection.push_back(Vector2(1.f, -1.f));
	mMisileDirection.push_back(Vector2(-1.f, 1.f));
	mMisileDirection.push_back(Vector2(1.f, 1.f));
	mMisileDirection.push_back(Vector2(-1.f, -1.f));
	mMisileDirection.push_back(Vector2(1.f, 0.f));
	mMisileDirection.push_back(Vector2(-1.f, 0.f));

	InitAnimation();

	// 렌더러
	SpriteRenderer* sr = AddComponent<SpriteRenderer>();
	shared_ptr<Mesh> mesh = ResourceManager::GetInstance()->Find<Mesh>(L"RectMesh");
	shared_ptr <Material> material = ResourceManager::GetInstance()->Find<Material>(L"FrozenOrbMaterial");
	sr->SetMesh(mesh);
	sr->SetMaterial(material);

	shared_ptr<Texture2D> tex = ResourceManager::GetInstance()->Find<Texture2D>(L"FrozenOrb");
	material->SetTexture(eTextureSlot::T0,tex);

	// 트랜스폼
	Transform* tr = GetComponent<Transform>();
	tr->SetScale(Vector3(2.f, 2.f, 1.f));
	
	// 제네릭 애니메이터
	AddComponent<GenericAnimator>();
}

void FrozenOrb::Update()
{
	Skil::Update();
}

void FrozenOrb::FixedUpdate()
{
	RunOrb();
	Skil::FixedUpdate();
}

void FrozenOrb::Render()
{
	Skil::Render();
}

void FrozenOrb::InitAnimation()
{
	Animator* animator = AddComponent<Animator>();
	shared_ptr<Texture2D> tex = ResourceManager::GetInstance()->Load<Texture2D>(L"FrozenOrb", L"Frozen//FrozenOrb.png");

	animator->Create(L"FrozenOrb", tex, Vector2(0.0f, 0.0f), Vector2(100.f, 100.f), Vector2(0.0f, 0.0f), 16, 0.1f);
	animator->Play(L"FrozenOrb");
}

void FrozenOrb::RunOrb()
{
	Transform* tr = GetComponent<Transform>();
	Vector3 pos = tr->GetPosition();

	mDirection.Normalize();
	pos += Vector3(mDirection.x, mDirection.y, 0.0f) * Time::GetInstance()->DeltaTime() * 2.f;

	tr->SetPosition(pos);
}

void FrozenOrb::OnOrb()
{
	GenericAnimator* animator = GetComponent<GenericAnimator>();
	if (animator->IsRunning())
		animator->Stop();

	AnimatorParam param;
	param.AnimType = eAnimType::Linear;
	param.StartValue = 0.0f;
	param.EndValue = 100.0f;
	param.DurationTime = 1.0f;


	int iSlice = 20;
	float fRadius = 0.3f;
	float fTheta = XM_2PI / (float)iSlice;

	vector<Vector2> pushDriection;
	for (int i = 0; i < iSlice; ++i)
	{
		Vector2 pos = Vector2
		(
			fRadius * cosf(fTheta * (float)i)
			, fRadius * sinf(fTheta * (float)i)
		);
		pushDriection.push_back(pos);
	}

	for (int i = 0; i < 20; ++i)
	{
		if (mFrozenMisile[i]->GetState() == eState::dead)
			continue;

		mFrozenMisile[i]->Active();
		mFrozenMisile[i]->SetDirection(pushDriection[i]);

		Vector3 pos = GetComponent<Transform>()->GetPosition();
		Transform* tr = mFrozenMisile[i]->GetComponent<Transform>();
		tr->SetPosition(pos);
	}

	param.DurationFunc = [this](float InCurValue)
	{
		for (int i = 0; i < mFrozenMisile.size(); ++i)
		{
			if(mFrozenMisile[i]->GetState() == eState::active)
				mFrozenMisile[i]->FixedUpdate();
		}
	};

	param.CompleteFunc = [this](float InCurValue)
	{
		RunningOrb();
	};

	animator->Start(param);
}

void FrozenOrb::RunningOrb()
{
	GenericAnimator* animator = GetComponent<GenericAnimator>();
	if (animator->IsRunning())
		animator->Stop();


}

void FrozenOrb::OffOrb()
{

}
