#include "CLightBolt.h"
#include "CAnimator.h"
#include "CResourceManager.h"
#include "CTransform.h"
#include "CSpriteRenderer.h"
#include "CObject.h"
#include "CLight.h"
#include "CGenericAnimator.h"
#include "CSceneManager.h"
#include "CLayer.h"
#include "CRenderer.h"
#include "CConstantBuffer.h"
#include "CCamera.h"
#include "CRenderer.h"
#include "CShockHit.h"
#include "CMonster.h"

#include "CSceneManager.h"
#include "CAudioClip.h"
#include "CAudioSource.h"
#include "CTime.h"

LightBolt::LightBolt()
	: BoltBase()
	, mArriveList {}
	, mCurObject(nullptr)
	, mNextObject(nullptr)
	, mDriectionTime(0.1f)
	, mRadius(600.f)
	, mBounceCount(0)
	, mBounceMaxCount(6)
	, mDirection(false) // false Y 가 줄어듬 true Y 가 늘어남
	, mYScale(0.0f)
	, RunSound(false)
	, Delta(0.5f)
{
	SetElementType(eElementType::HitLight);
	mCost = 30.f;
}

LightBolt::~LightBolt()
{
}

void LightBolt::Initalize()
{
	// 사이즈
	Transform* tr = GetComponent<Transform>();
	tr->SetSize(Vector3(5000, 5000, 1.0f));

	// 애니메이터 초기화
	InitAnimation();

	// 렌더러
	SpriteRenderer* sr = AddComponent<SpriteRenderer>();
	weak_ptr<Mesh> mesh = ResourceManager::GetInstance()->Find<Mesh>(L"RectMesh");
	weak_ptr<Material> material = ResourceManager::GetInstance()->Find<Material>(L"LightBoltMaterial");
	weak_ptr<Texture2D> tex = ResourceManager::GetInstance()->Find<Texture2D>(L"LightBolt");
	material.lock()->SetTexture(eTextureSlot::T0, tex);

	sr->SetMesh(mesh);
	sr->SetMaterial(material);

	// 제네릭애니메이터
	AddComponent<GenericAnimator>();

	AudioSource* audio = AddComponent<AudioSource>();
	std::weak_ptr<AudioClip> clip = ResourceManager::GetInstance()->Load<AudioClip>(L"LightBoltSound", L"SoundResource\\novaice.wav");
	audio->SetClip(clip);
	audio->SetLoop(false);
}

void LightBolt::Update()
{
	if (mOwner == nullptr)
		return;

	if (GetRun() == false)
	{
		BaseRenderer* renderer = GetComponent<BaseRenderer>();
		if (renderer == nullptr)
			return;

		renderer->SetRenderStop(true);

		if (mOwner == nullptr)
			return;

		Animator* animator = mOwner->GetComponent<Animator>();
		if (animator == nullptr)
			return;

		if (!animator->GetPlayAnimation()->IsComplete())
			return;

		SetRun(true);
		renderer->SetRenderStop(false);
	}

	if (mCurObject == nullptr)
		return;

	if (Delta >= 0.0f)
	{
		Delta -= Time::GetInstance()->DeltaTime();
		if (Delta < 0.0f)
		{
			GetComponent<AudioSource>()->Stop();
		}
	}

	if (mNextObject == nullptr)
	{
		Transform* Tr = GetComponent<Transform>();
		Transform* OwnerTr = mOwner->GetComponent<Transform>();
		Transform* MonsterTr = mCurObject->GetComponent<Transform>();

		Vector3 OwnerPos = OwnerTr->GetPosition();
		Vector3 MonsterPos = MonsterTr->GetPosition();

		Vector3 diff = MonsterPos - OwnerPos;
		Vector3 CurPos = OwnerPos + (diff * 0.5f);
		CurPos.z = 1.0f;

		Tr->SetPosition(CurPos);

		Vector3 Size = Tr->GetSize();
		Size.y = diff.Length();
		Size.x = 200.f;

		Tr->SetSize(Size);

		Angle(Vector2(MonsterPos.x, MonsterPos.y));
	}
	else
	{
		if (mCurObject == nullptr || mNextObject == nullptr)
			return;

		Transform* Tr = GetComponent<Transform>();
		Transform* NextTr = mNextObject->GetComponent<Transform>();
		Transform* MonsterTr = mCurObject->GetComponent<Transform>();

		Vector3 NextPos = NextTr->GetPosition();
		Vector3 MonsterPos = MonsterTr->GetPosition();

		Vector3 diff = NextPos - MonsterPos;
		Vector3 CurPos = MonsterPos + (diff * 0.5f);
		CurPos.z = 1.0f;

		Tr->SetPosition(CurPos);

		Vector3 Size = Tr->GetSize();
		Size.y = diff.Length();
		Size.x = 200.f;

		Tr->SetSize(Size);

		Angle(Vector2(NextPos.x, NextPos.y));
	}


	Skil::Update();
}

void LightBolt::FixedUpdate()
{
	BoltBase::FixedUpdate();
}

void LightBolt::Render()
{
	Renderer::LightingBoltCB Info = {};
	Info.BoltType = (UINT)GetElementType();
	Info.Driection = mDirection;
	Info.vValue = mYScale;

	ConstantBuffer* cb = Renderer::constantBuffers[(UINT)eCBType::LightingData];
	cb->SetData(&Info);
	cb->Bind(eShaderStage::PS);

	BoltBase::Render();

	// 초기화
	/*cb->SetData(&Info);
	cb->Bind(eShaderStage::PS);*/
}

void LightBolt::InitAnimation()
{
	Animator* animator = AddComponent<Animator>();

	weak_ptr<Texture2D> tex =
		ResourceManager::GetInstance()->Load<Texture2D>(L"LightBolt", L"LightBolt//LightBolt.png");

	animator->Create(L"LightBolt", tex, Vector2::Zero, Vector2(43.f, 85.f), Vector2::Zero, 8, 0.1f);
	animator->Play(L"LightBolt");
}

void LightBolt::SearchArriveTarget()
{
	if (mBounceCount >= mBounceMaxCount)
	{
		mBounceCount = 0;
		mCurObject = nullptr;
		GetComponent<AudioSource>()->Stop();
		Death();
		return;
	}

	Layer& layer = SceneManager::GetInstance()->GetActiveScene()->GetLayer(eLayerType::Monster);
	const vector<GameObject*>& Objects = layer.GetGameObjects();

	Transform* tr = mCurObject->GetComponent<Transform>();
	Vector3 Pos = tr->GetPosition();

	Camera* camera = Renderer::mainCamera;
	Transform* cameraTr = camera->GetOwner()->GetComponent<Transform>();
	Vector3 CameraPos = cameraTr->GetPosition();
	Vector2 WindowSize = Vector2(1600.f, 900.f);

	// 현재 위치에 인접한 몬스터 탐색
	std::unordered_map<float, GameObject*> arriveList;
	for (GameObject* obj : Objects)
	{
		if (obj == nullptr)
			continue;

		if (mCurObject == nullptr)
			continue;

		if (obj == mCurObject)
			continue;


		if (obj->GetState() != eState::active)
			continue;

		if (mArriveList.find(obj) != mArriveList.end())
			continue;

		const Transform* objTr = obj->GetComponent<Transform>();
		Vector3 objPos = objTr->GetPosition();

		/*if (objPos.x < CameraPos.x - (WindowSize.x * 0.5f) || objPos.x > CameraPos.x + (WindowSize.x * 0.5f))
			continue;

		if (objPos.y < CameraPos.y - (WindowSize.y * 0.5f) || objPos.y > CameraPos.y + (WindowSize.y * 0.5f))
			continue;*/

		Vector3 Vec = objPos - Pos;

		float len = Vec.Length();
		if (len < mRadius)
			arriveList.insert(std::make_pair(len,obj));
	}


	// 인접한 몬스터를 찾지못한경우 메모리 해제
	if (arriveList.size() < 1)
	{
		GetComponent<AudioSource>()->Stop();
		Death();
		return;
	}

	mNextObject = arriveList.begin()->second;

	// 다음 갈위치와 연결
	//float ySize = fabs(Pos.y - mNextObject->GetComponent<Transform>()->GetPosition().y);

	//Transform* LightTr = GetComponent<Transform>();
	//Vector3 Size = LightTr->GetSize();
	//Size.y = ySize;
	//LightTr->SetSize(Size);

	//// 다음 갈 위치 와 현위치 중간지점에 위치한다
	//Vector3 finalPos = mNextObject->GetComponent<Transform>()->GetPosition() - Pos;
	//Pos += (finalPos * 0.5f);

	//LightTr->SetPosition(Pos);

	LinghtingRun();
}

void LightBolt::LightingOn(GameObject* object)
{
	SetCurObject(object);
	LinghtingRun();
}

void LightBolt::LinghtingRun()
{
	mBounceCount++;
	if (mBounceCount > mBounceMaxCount)
		return;

	if (mCurObject == nullptr)
		return;

	GenericAnimator* generic = GetComponent<GenericAnimator>();
	if (generic->IsRunning())
		generic->Stop();

	AnimatorParam param;
	param.AnimType = eAnimType::Linear;
	param.StartValue = 0.0f;
	param.EndValue = 1.0f;
	param.DurationTime = 0.15f;

	// 각도 세팅
	Transform* Tr = mCurObject->GetComponent<Transform>();
	Vector3 Pos = Tr->GetPosition();
	Angle(Vector2(Pos.x,Pos.y));

	if (mNextObject != nullptr)
	{
		Transform* mTr = GetComponent<Transform>();
		mTr->SetPosition(Pos);

		Transform* nextTr = mNextObject->GetComponent<Transform>();
		Vector3 nextPos = nextTr->GetPosition();

		Angle(Vector2(nextPos.x, nextPos.y));
	}

	param.DurationFunc = [this](float InCurValue)
	{
		mDirection = false;
		mYScale = InCurValue;
	};

	param.CompleteFunc = [this](float InCurValue)
	{
		AudioSource* audio = GetComponent<AudioSource>();
		audio->Stop();
		audio->Play(0.2f);
		Monster* monster = dynamic_cast<Monster*>(mCurObject);
		if (monster != nullptr)
		{
			//monster->GetHit();
		}

		ShockHit* shock = Object::Instantiate<ShockHit>(eLayerType::Effect, true);
		shock->SetTarget(mCurObject);

		if(mNextObject != nullptr)
			mCurObject = mNextObject;

		HitSkil(mCurObject);
		mArriveList.insert(mCurObject);
		SearchArriveTarget();
	};

	generic->Start(param);
}

void LightBolt::LightningEnd()
{
	if (mBounceCount > mBounceMaxCount)
		return;

	GenericAnimator* generic = GetComponent<GenericAnimator>();
	if (generic->IsRunning())
		generic->Stop();

	AnimatorParam param;
	param.AnimType = eAnimType::Linear;
	param.StartValue = 1.0f;
	param.EndValue = 0.0f;
	param.DurationTime = 0.1f;

	param.DurationFunc = [this](float InCurValue)
	{
		mDirection = true;
		mYScale = InCurValue;
	};

	param.CompleteFunc = [this](float InCurValue)
	{
		ShockHit* shock = Object::Instantiate<ShockHit>(eLayerType::Effect, true);
		shock->SetTarget(mCurObject);

		if (mNextObject != nullptr)
			mCurObject = mNextObject;

		mArriveList.insert(mCurObject);
		SearchArriveTarget();
	};

	generic->Start(param);
}

void LightBolt::HitSkil(GameObject* obj)
{
	Monster* monster = dynamic_cast<Monster*>(obj);

	if (monster == nullptr)
		return;

	float hp = monster->GetHP();
	hp -= 20.f;

	monster->SetHP(hp);
	monster->SetMonsterStatusEffect(eElementType::HitLight);
}
