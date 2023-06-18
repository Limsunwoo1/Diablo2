#include "CMeteor.h"
#include "CTransform.h"
#include "CSpriteRenderer.h"
#include "CAnimator.h"
#include "CFireTargetPin.h"
#include "CFlames.h"
#include "CObject.h"

#include "CTime.h"
#include "CResourceManager.h"
#include "CCollisionManager.h"
#include "CMonster.h"
#include "CCollider2D.h"
#include "CLayer.h"
#include "CSceneManager.h"

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

	// 사이즈
	Transform* tr = GetComponent<Transform>();
	tr->SetSize(Vector3(1000.f, 800.f, 1.f));

	// 제네릭 애니메이터
	AddComponent<GenericAnimator>();

	// 애니메이터 초기화
	InitAnimation();

	// 렌더러
	SpriteRenderer* sr = AddComponent<SpriteRenderer>();
	weak_ptr<Mesh> mesh = ResourceManager::GetInstance()->Find<Mesh>(L"RectMesh");
	weak_ptr<Material> material = ResourceManager::GetInstance()->Find<Material>(L"MeteorMaterial");
	weak_ptr<Texture2D> tex = ResourceManager::GetInstance()->Find<Texture2D>(L"Meteor");
	material.lock()->SetTexture(eTextureSlot::T0, tex);
	tex = ResourceManager::GetInstance()->Find<Texture2D>(L"MeteorEnd");
	material.lock()->SetTexture(eTextureSlot::T2, tex);

	sr->SetMesh(mesh);
	sr->SetMaterial(material);

	Collider2D* col = AddComponent<Collider2D>();
	col->SetType(eColliderType::Rect);
	col->SetSize(Vector2(0.25f, 0.25f));
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

	animator->Create(L"Meteor", tex, Vector2::Zero, Vector2(30.f, 45.f), Vector2::Zero, 5, 0.2f);
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

	// 핀오브젝트 활성화
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
		Vector3 size = (mTr->GetSize() * 0.5f);

		Vec = mPinPos - (Pos - size);
		if (mPinPos.y >= Pos.y /*&& fabs(Vec.x) < 0.005f*/)
		{
			genericAnimator->Stop(true);
			return;
		}


		// 일정시간후 메테오 낙하
		if (runLine < InCurValue)
		{
			Vec.Normalize();
			Pos.y += -1.0f * Time::GetInstance()->DeltaTime() * mSpeed * 0.5f;
			mTr->SetPosition(Pos);
		}
	};

	param.CompleteFunc = [this](float InCurValue)
	{
		Layer& layer = SceneManager::GetInstance()->GetActiveScene()->GetLayer(eLayerType::Monster);
		const std::vector<GameObject*>& Objects = layer.GetGameObjects();

		Collider2D* col = GetComponent<Collider2D>();
		if (col != nullptr)
		{
			for (GameObject* obj : Objects)
			{
				if (obj == nullptr)
					continue;

				bool collistion = false;
				collistion = CollisionManager::GetInstance()->AABBRect_VS_Rect(col, obj->GetComponent<Collider2D>());

				if (collistion)
				{
					HitSkil(obj);
				}
			}

		}
		mTargetPin->Paused();
		OffMeteor();
		// 애니메이션 교체
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

	// 메테오 충돌위치 불길생성
	Transform* tr = mFlames[0]->GetComponent<Transform>();
	tr->SetPosition(mPinPos);
	mFlames[0]->Active();

	float radius = 100.f;
	float arrX[9] = {0.f, -radius, radius, 0.f, 0.f, radius, -radius, -radius, radius};
	float arrY[9] = {0.f, 0.f, 0.f, radius, -radius, radius, radius, -radius, -radius};

	float fTheta = XM_PIDIV2 / (float)5.f;

	// 랜덤한 위치에 불길 생성
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
		// 메테오오브젝트 애니메이션 끝나면 렌더 스탑
		Animator* animator = GetComponent<Animator>();
		if (animator->GetPlayAnimation()->IsComplete())
		{
			SpriteRenderer* sr = GetComponent<SpriteRenderer>();
			sr->SetRenderStop();
		}

		// 폭발 렌더후 불길 애니메이션 렌더
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


void Meteor::HitSkil(GameObject* obj)
{
	Monster* monster = dynamic_cast<Monster*>(obj);

	if (monster == nullptr)
		return;

	float hp = monster->GetHP();
	hp -= 35.f;

	monster->SetHP(hp);
}