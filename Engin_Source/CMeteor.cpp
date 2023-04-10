#include "CMeteor.h"
#include "CTransform.h"
#include "CSpriteRenderer.h"
#include "CAnimator.h"
#include "CFireTargetPin.h"
#include "CFlames.h"

#include "CTime.h"
#include "CResourceManager.h"


Meteor::Meteor()
	: Skil()
	, mbRun(false)
{
}

Meteor::~Meteor()
{
	delete mTargetPin;
	mTargetPin = nullptr;

	for (Flames* flames : mFlames)
	{
		if (flames == nullptr)
			continue;

		delete flames;
		flames = nullptr;
	}
	mFlames.clear();
}

void Meteor::Initalize()
{
	mTargetPin = new FireTargetPin();
	mTargetPin->Initalize();

	for (int i = 0; i < 8; ++i)
	{
		Flames* flames = new Flames();
		flames->Initalize();

		mFlames.push_back(flames);
	}

	// 사이즈
	Transform* tr = GetComponent<Transform>();
	tr->SetScale(Vector3(2.0f, 2.0f, 1.0f));

	// 제네릭 애니메이터
	AddComponent<GenericAnimator>();

	// 애니메이터 초기화
	InitAnimation();

	// 렌더러
	SpriteRenderer* sr = AddComponent<SpriteRenderer>();
	shared_ptr<Mesh> mesh = ResourceManager::GetInstance()->Find<Mesh>(L"RectMesh");
	shared_ptr<Material> material = ResourceManager::GetInstance()->Find<Material>(L"MeteorMaterial");
	shared_ptr<Texture2D> tex = ResourceManager::GetInstance()->Find<Texture2D>(L"Meteor");
	material->SetTexture(eTextureSlot::T0,tex);
	sr->SetMesh(mesh);
	sr->SetMaterial(material);
}

void Meteor::Update()
{
	if (!mbRun)
	{
		OnMeteor();
		mbRun = true;
	}

	if(mTargetPin)
		mTargetPin->Update();

	for (Flames* flames : mFlames)
	{
		if (flames == nullptr)
			continue;

		flames->Update();
	}

	GameObject::Update();
}

void Meteor::FixedUpdate()
{
	if (mTargetPin)
		mTargetPin->FixedUpdate();

	for (Flames* flames : mFlames)
	{
		if (flames == nullptr)
			continue;

		flames->FixedUpdate();
	}

	GameObject::FixedUpdate();
}

void Meteor::Render()
{
	if (mTargetPin)
		mTargetPin->Render();

	for (Flames* flames : mFlames)
	{
		if (flames == nullptr)
			continue;

		flames->Render();
	}

	GameObject::Render();
}

void Meteor::InitAnimation()
{
	Animator* animator = AddComponent<Animator>();

	shared_ptr<Texture2D> tex = 
		ResourceManager::GetInstance()->Load<Texture2D>(L"Meteor", L"Meteor//MeteorSprite.png");


	animator->Create(L"Meteor", tex, Vector2::Zero, Vector2(100, 100.f), Vector2::Zero, 45, 0.05f);
	animator->Play(L"Meteor");
}


void Meteor::RunMeteor()
{
	
}

void Meteor::SetSpeed()
{
	Transform* mTr = GetComponent<Transform>();
	Vector3 Pos = mTr->GetPosition();

	mSpeed = abs(mPinPos.y - Pos.y);
}

void Meteor::OnMeteor()
{
	GenericAnimator* genericAnimator = GetComponent<GenericAnimator>();

	if (genericAnimator->IsRunning())
		genericAnimator->Stop();

	Transform* pinTr = mTargetPin->GetComponent<Transform>();
	pinTr->SetPosition(mPinPos);

	AnimatorParam param;
	param.AnimType = eAnimType::Linear;
	param.StartValue = 0.0f;
	param.EndValue = 1.0f;
	param.DurationTime = 2.0f;

	param.DurationFunc = [this, genericAnimator](float InCurValue)
	{
		Vector3 Vec;
		Transform* mTr = GetComponent<Transform>();
		Vector3 Pos = mTr->GetPosition();

		Vec = mPinPos - Pos;
		if (fabs(Vec.y) < 0.05f)
		{
			Animator* animator = GetComponent<Animator>();
			//animator->Play(L"EndMeteor", false);

			genericAnimator->Stop(true);
			return;
		}

		Vec.Normalize();
		Pos += Vec * Time::GetInstance()->DeltaTime() * mSpeed * 3.0f;
		mTr->SetPosition(Pos);
	};

	param.CompleteFunc = [this](float InCurValue)
	{
		mTargetPin->Paused();
		OffMetor();
		// 애니메이션 교체
	};

	genericAnimator->Start(param);
}

void Meteor::OffMetor()
{
	// 메테오 충돌위치 불길생성
	Transform* tr = mFlames[0]->GetComponent<Transform>();
	tr->SetPosition(mPinPos);
	mFlames[0]->Active();

	float arrX[8] = { -1.f, 1.f, 0.f, 0.f, 1.f, -1.f, -1.f, 1.f};
	float arrY[8] = { 0.f, 0.f, 1.f, -1.f, 1.f, 1.f, -1.f, -1.f};
	for (int i = 0; i < mFlames.size(); ++i)
	{
		mFlames[i]->Active();

		Transform* tr = mFlames[i]->GetComponent<Transform>();
		Vector3 pos = tr->GetPosition();
		pos.x = mPinPos.x + arrX[i];
		pos.y = mPinPos.y + arrY[i];

		tr->SetPosition(pos);
	}

	GenericAnimator* genericAnimator = GetComponent<GenericAnimator>();

	AnimatorParam param;
	param.AnimType = eAnimType::Linear;
	param.StartValue = 0.0f;
	param.EndValue = 1.0f;
	param.DurationTime = 2.0f;

	param.DurationFunc = [this](float InCurValue)
	{
		// 렌더링 정지
		Animator* animator = GetComponent<Animator>();
		if (animator->GetPlayAnimation()->IsComplete())
		{
			SpriteRenderer* sr = GetComponent<SpriteRenderer>();
			sr->GetMaterial()->Clear();
			this->Paused();
		}
	};

	param.CompleteFunc = [this](float InCurValue)
	{
		for (Flames* flame : mFlames)
		{
			flame->Death();
		}

		this->Death();
	};

	genericAnimator->Start(param);
}

void Meteor::SetPinPos(Math::Vector3& pos)
{
	mPinPos = pos; 
	SetSpeed();
}
void Meteor::SetPinPos(float& x, float& y, float& z) 
{
	mPinPos = Vector3(x, y, z); 
	SetSpeed();
}
