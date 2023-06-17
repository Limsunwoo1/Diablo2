#include "CFrozenOrb.h"
#include "CFrozenMissile.h"
#include "CAnimator.h"
#include "CSpriteRenderer.h"
#include "CGenericAnimator.h"

#include "CTime.h"
#include "CResourceManager.h"

FrozenOrb::FrozenOrb()
	: Skil()
	, mFrozenMissile{}
	, mSpeakerIndex(0)
	, mRunningTime(0.0f)
	, mbOff(false)
	, mbOnOrb(false)
{
}

FrozenOrb::~FrozenOrb()
{
	for (FrozenMissile* obj : mFrozenMissile)
	{
		if (obj == nullptr)
			continue;

		delete obj;
		obj = nullptr;
	}

	for (FrozenMissile* obj : mSpeakerMissile)
	{
		if (obj == nullptr)
			continue;

		delete obj;
		obj = nullptr;
	}
}

void FrozenOrb::Initalize()
{
	for (int i = 0; i < 20; ++i)
		mFrozenMissile.emplace_back(new FrozenMissile);
	for (int i = 0; i < 20; ++i)
		mFrozenMissile[i]->Initalize();

	for (int i = 0; i < 20; ++i)
		mSpeakerMissile.emplace_back(new FrozenMissile);
	for (int i = 0; i < 20; ++i)
		mSpeakerMissile[i]->Initalize();

	InitAnimation();

	// 렌더러
	SpriteRenderer* sr = AddComponent<SpriteRenderer>();
	weak_ptr<Mesh> mesh = ResourceManager::GetInstance()->Find<Mesh>(L"RectMesh");
	weak_ptr <Material> material = ResourceManager::GetInstance()->Find<Material>(L"FrozenOrbMaterial");
	sr->SetMesh(mesh);
	sr->SetMaterial(material);

	sr->SetRenderStop();

	weak_ptr<Texture2D> tex = ResourceManager::GetInstance()->Find<Texture2D>(L"FrozenOrb");
	material.lock()->SetTexture(eTextureSlot::T0, tex);

	tex = ResourceManager::GetInstance()->Find<Texture2D>(L"FrozenOrbEnd");
	material.lock()->SetTexture(eTextureSlot::T2, tex);

	// 트랜스폼
	Transform* tr = GetComponent<Transform>();
	tr->SetSize(Vector3(140.f, 140.f, 1.0f));
	
	Vector3 Pos = tr->GetPosition();
	tr->SetPosition(Vector3(Pos.x, Pos.y, 0.8f));
	
	// 제네릭 애니메이터
	AddComponent<GenericAnimator>();

	//
	//this->Paused();
}

void FrozenOrb::Update()
{
	if (mOwner)
	{
		Animator* animator = mOwner->GetComponent<Animator>();

		if (animator->GetPlayAnimation()->IsComplete())
		{
			//this->Active();

			OnOrb();
			mOwner = nullptr;
			mbOnOrb = true;

			SpriteRenderer* sr = GetComponent<SpriteRenderer>();
			sr->SetRenderStop(false);
			return;
		}
	}

	Skil::Update();

	for (FrozenMissile* obj : mFrozenMissile)
	{
		if (obj->GetState() != eState::active)
			continue;

		obj->Update();
	}


	for (FrozenMissile* obj : mSpeakerMissile)
	{
		if (obj->GetState() != eState::active)
			continue;

		obj->Update();
	}
}

void FrozenOrb::FixedUpdate()
{
	if(!mbOff && mbOnOrb)
		RunOrb();

	Skil::FixedUpdate();

	for (FrozenMissile* obj : mFrozenMissile)
	{
		if (obj->GetState() != eState::active)
			continue;

		obj->FixedUpdate();
	}

	for (FrozenMissile* obj : mSpeakerMissile)
	{
		if (obj->GetState() != eState::active)
			continue;

		obj->FixedUpdate();
	}
}

void FrozenOrb::Render()
{
	for (FrozenMissile* obj : mFrozenMissile)
	{
		if (obj->GetState() != eState::active)
			continue;

		obj->Render();
	}

	for (FrozenMissile* obj : mSpeakerMissile)
	{
		if (obj->GetState() != eState::active)
			continue;

		obj->Render();
	}

	Skil::Render();
}

void FrozenOrb::InitAnimation()
{
	Animator* animator = AddComponent<Animator>();
	weak_ptr<Texture2D> tex = ResourceManager::GetInstance()->Load<Texture2D>(L"FrozenOrb", L"Frozen//FrozenOrb2.png");
	weak_ptr<Texture2D> texture = ResourceManager::GetInstance()->Load<Texture2D>(L"FrozenOrbEnd", L"Frozen//FrozenOrbEnd.png");

	animator->Create(L"FrozenOrb", tex, Vector2(0.0f, 0.0f), Vector2(100.f, 100.f), Vector2(0.0f, 0.0f), 16, 0.1f);
	animator->Create(L"FrozenOrbEnd", texture, Vector2(0.0f, 0.0f), Vector2(100.f, 100.f), Vector2(0.0f, 0.0f), 18, 0.1f);
	animator->Play(L"FrozenOrb");
}

void FrozenOrb::RunOrb()
{
	Transform* tr = GetComponent<Transform>();
	Vector3 pos = tr->GetPosition();

	mDirection.Normalize();
	pos += Vector3(mDirection.x, mDirection.y, 0.0f) * Time::GetInstance()->DeltaTime() * 250.f;

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
	param.EndValue = 1.0f;
	param.DurationTime = 1.5;


	int iSlice = 20;
	float fTheta = 360.f / (float)iSlice;

	vector<Vector2> pushDriection;
	for (int i = 0; i < iSlice; ++i)
	{
		// 회전
		mFrozenMissile[i]->GetComponent<Transform>()->SetRotation(Vector3(0.0f, 0.0f, iSlice * (float)i));
		mFrozenMissile[i]->SetMode(eMisileMode::Rotation);
	}

	float startLine = 0.1f;
	float endLine = 0.9f;

	param.DurationFunc = [this, startLine, endLine](float InCurValue)
	{
		if (InCurValue >= startLine)
		{
			for (int i = 0; i < 20; ++i)
			{
				if (mFrozenMissile[i]->GetState() == eState::active)
					continue;

				mFrozenMissile[i]->Active();

				Vector3 pos = GetComponent<Transform>()->GetPosition();
				Transform* tr = mFrozenMissile[i]->GetComponent<Transform>();
				tr->SetPosition(pos);
			}
		}
		
		if(InCurValue >= startLine + 0.1f && InCurValue  <= endLine)
		{
			RunningOrb();
		}

		if (InCurValue >= endLine)
		{
			for (int i = 0; i < 20; ++i)
			{
				mFrozenMissile[i]->Death();
			}
		}
	};

	param.CompleteFunc = [this](float InCurValue)
	{
		for (int i = 0; i < 20; ++i)
		{
			mFrozenMissile[i]->Active();

			Vector3 pos = GetComponent<Transform>()->GetPosition();
			Transform* tr = mFrozenMissile[i]->GetComponent<Transform>();
			tr->SetPosition(pos);
		}

		OffOrb();
	};

	animator->Start(param);
}

void FrozenOrb::RunningOrb()
{
	if (mSpeakerIndex >= mSpeakerMissile.size())
		return;

	mRunningTime += Time::GetInstance()->DeltaTime();
	if (mRunningTime >= 0.05f)
	{
		mRunningTime -= 0.05f;

		mSpeakerMissile[mSpeakerIndex]->Active();

		float fTheta = 360.f / 8.f;
		Vector3 pos = GetComponent<Transform>()->GetPosition();
		Transform* tr = mSpeakerMissile[mSpeakerIndex]->GetComponent<Transform>();
		tr->SetPosition(pos);
		tr->SetRotation(Vector3(0.0f, 0.0f, mSpeakerIndex * fTheta));


		if(mSpeakerIndex < mSpeakerMissile.size())
			mSpeakerIndex++;
	}
}

void FrozenOrb::OffOrb()
{
	GenericAnimator* animator = GetComponent<GenericAnimator>();
	if (animator->IsRunning())
		animator->Stop();

	AnimatorParam param;
	param.AnimType = eAnimType::Linear;
	param.StartValue = 0.0f;
	param.EndValue = 1.0f;
	param.DurationTime = 5.0f;


	int iSlice = 20;
	float fTheta = 360.f / (float)iSlice;

	vector<Vector2> pushDriection;
	for (int i = 0; i < iSlice; ++i)
	{
		/*Vector2 rotation = Vector2
		(
			fRadius * cosf(fTheta * (float)i)
			, fRadius * sinf(fTheta * (float)i)
		);*/

		// 회전
		mFrozenMissile[i]->GetComponent<Transform>()->SetRotation(Vector3(0.0f, 0.0f, iSlice * (float)i));
		mFrozenMissile[i]->SetTime(0.0f);
		mFrozenMissile[i]->SetMode(eMisileMode::ReversRotation);
	}

	Animator* ani = GetComponent<Animator>();
	ani->Play(L"FrozenOrbEnd", false);

	mbOff = true;

	Transform* tr = GetComponent<Transform>();
	Vector3 size = tr->GetScale();
	tr->SetScale(Vector3(size.x * 2, size.y * 2, size.z));

	float startLine = 0.05f;
	param.DurationFunc = [this, startLine](float InCurValue)
	{
		if (InCurValue * 20.f >= startLine)
		{
			for (int i = 0; i < 20; ++i)
			{
				if (mFrozenMissile[i]->GetState() == eState::active)
					continue;

				mFrozenMissile[i]->Active();

				Vector3 pos = GetComponent<Transform>()->GetPosition();
				Transform* tr = mFrozenMissile[i]->GetComponent<Transform>();
				tr->SetPosition(pos);
			}
		}
	};

	param.CompleteFunc = [this](float InCurValue)
	{
		Death();
	};

	animator->Start(param);
}
