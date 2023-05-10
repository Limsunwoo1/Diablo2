#include "CMeteor.h"
#include "CTransform.h"
#include "CSpriteRenderer.h"
#include "CAnimator.h"
#include "CFireTargetPin.h"
#include "CFlames.h"
#include "CObject.h"

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

	for (int i = 0; i < 10; ++i)
	{
		Flames* flames = new Flames();
		flames->Initalize();

		mFlames.push_back(flames);
	}

	// ������
	Transform* tr = GetComponent<Transform>();
	tr->SetScale(Vector3(6.0f, 2.0f, 1.0f));

	// ���׸� �ִϸ�����
	AddComponent<GenericAnimator>();

	// �ִϸ����� �ʱ�ȭ
	InitAnimation();

	// ������
	SpriteRenderer* sr = AddComponent<SpriteRenderer>();
	weak_ptr<Mesh> mesh = ResourceManager::GetInstance()->Find<Mesh>(L"RectMesh");
	weak_ptr<Material> material = ResourceManager::GetInstance()->Find<Material>(L"MeteorMaterial");
	weak_ptr<Texture2D> tex = ResourceManager::GetInstance()->Find<Texture2D>(L"Meteor");
	material.lock()->SetTexture(eTextureSlot::T0, tex);
	tex = ResourceManager::GetInstance()->Find<Texture2D>(L"MeteorEnd");
	material.lock()->SetTexture(eTextureSlot::T2, tex);

	sr->SetMesh(mesh);
	sr->SetMaterial(material);
}

void Meteor::Update()
{
	if (!mbRun)
	{
		if (mOwner)
		{
			Animator* animator = mOwner->GetComponent<Animator>();

			if (!animator->GetPlayAnimation()->IsComplete())
			{
				return;
			}
		}

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
	GameObject::Render();

	if (mTargetPin)
		mTargetPin->Render();

	for (Flames* flames : mFlames)
	{
		if (flames == nullptr)
			continue;

		flames->Render();
	}
}

void Meteor::InitAnimation()
{
	Animator* animator = AddComponent<Animator>();

	weak_ptr<Texture2D> tex =
		ResourceManager::GetInstance()->Load<Texture2D>(L"Meteor", L"Meteor//MeteorDrop.png");

	animator->Create(L"Meteor", tex, Vector2::Zero, Vector2(100, 100.f), Vector2::Zero, 9, 0.1f);
	animator->Play(L"Meteor");

	tex = ResourceManager::GetInstance()->Load<Texture2D>(L"MeteorEnd", L"Meteor//MeteorEnd.png");
	animator->Create(L"MeteorEnd", tex, Vector2::Zero, Vector2(100, 100.f), Vector2::Zero, 29, 0.05f);
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

	// �ɿ�����Ʈ Ȱ��ȭ
	Transform* pinTr = mTargetPin->GetComponent<Transform>();
	pinTr->SetPosition(mPinPos);
	mTargetPin->Active();

	float runLine = 0.2f;

	AnimatorParam param;
	param.AnimType = eAnimType::Linear;
	param.StartValue = 0.0f;
	param.EndValue = 1.0f;
	param.DurationTime = 4.0f;

	param.DurationFunc = [this, genericAnimator, runLine](float InCurValue)
	{
		Vector3 Vec;
		Transform* mTr = GetComponent<Transform>();
		Vector3 Pos = mTr->GetPosition();

		Vec = mPinPos - Pos;
		if (fabs(Vec.y) < 0.05f /*&& fabs(Vec.x) < 0.005f*/)
		{
			genericAnimator->Stop(true);
			return;
		}

		// �����ð��� ���׿� ����
		if (runLine < InCurValue)
		{
			Vec.Normalize();
			Pos += Vec * Time::GetInstance()->DeltaTime() * mSpeed * 3.0f;
			mTr->SetPosition(Pos);
		}
	};

	param.CompleteFunc = [this](float InCurValue)
	{
		mTargetPin->Paused();
		OffMeteor();
		// �ִϸ��̼� ��ü
	};

	genericAnimator->Start(param);
}

void Meteor::OffMeteor()
{
	GenericAnimator* genericAnimator = GetComponent<GenericAnimator>();

	if (genericAnimator->IsRunning())
		genericAnimator->Stop();

	Animator* animator = GetComponent<Animator>();
	animator->Play(L"MeteorEnd", false);

	// ���׿� �浹��ġ �ұ����
	Transform* tr = mFlames[0]->GetComponent<Transform>();
	tr->SetPosition(mPinPos);
	mFlames[0]->Active();

	float radius = 0.5f;
	float arrX[9] = {0.f, -radius, radius, 0.f, 0.f, radius, -radius, -radius, radius};
	float arrY[9] = {0.f, 0.f, 0.f, radius, -radius, radius, radius, -radius, -radius};

	float fTheta = XM_PIDIV2 / (float)5.f;

	// ������ ��ġ�� �ұ� ����
	for (int i = 0; i < mFlames.size(); ++i)
	{
		mFlames[i]->Active();

		Transform* tr = mFlames[i]->GetComponent<Transform>();
		Vector3 pos = tr->GetPosition();
		pos.x = mPinPos.x + (cosf(fTheta * (float)i * mFlames[i]->GetID()) * radius + arrX[i]);
		pos.y = mPinPos.y + (sinf(fTheta * (float)i * mFlames[i]->GetID()) * radius + arrY[i]);

		tr->SetPosition(pos);
	}

	AnimatorParam param;
	param.AnimType = eAnimType::Linear;
	param.StartValue = 0.0f;
	param.EndValue = 1.0f;
	param.DurationTime = 10.0f;

	param.DurationFunc = [this](float InCurValue)
	{
		// ���׿�������Ʈ �ִϸ��̼� ������ ���� ��ž
		Animator* animator = GetComponent<Animator>();
		if (animator->GetPlayAnimation()->IsComplete())
		{
			SpriteRenderer* sr = GetComponent<SpriteRenderer>();
			sr->SetRenderStop();
		}

		// ���� ������ �ұ� �ִϸ��̼� ����
		for (Flames* flame : mFlames)
		{
			if (flame == nullptr)
				continue;

			Animator* flameAnimator = flame->GetComponent<Animator>();
			if (flameAnimator->GetPlayAnimation()->IsComplete())
			{
				flameAnimator->Play(L"Flame");
				//flame->GetComponent<SpriteRenderer>()->SetRenderStop();
			}
		}
	};

	param.CompleteFunc = [this](float InCurValue)
	{
		for (Flames* flame : mFlames)
		{
			flame->Death();
		}

		Object::ObjectDestroy(this);
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
