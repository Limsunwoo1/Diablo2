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

LightBolt::LightBolt()
	: BoltBase()
	, mAriiveList {}
	, mCurObject(nullptr)
	, mDriectionTime(0.1f)
	, mRadius(400.f)
	, mBounceCount(0)
	, mBounceMaxCount(3)
	, mDirection(false) // false Y �� �پ�� true Y �� �þ
	, mYScale(0.0f)
{
	bool mDirection;
	float mYScale;
	SetElementType(eElementType::HitLight);
}

LightBolt::~LightBolt()
{
}

void LightBolt::Initalize()
{
	// ������
	Transform* tr = GetComponent<Transform>();
	tr->SetSize(Vector3(320.f, 320.f, 1.0f));

	// �ִϸ����� �ʱ�ȭ
	InitAnimation();

	// ������
	SpriteRenderer* sr = AddComponent<SpriteRenderer>();
	weak_ptr<Mesh> mesh = ResourceManager::GetInstance()->Find<Mesh>(L"RectMesh");
	weak_ptr<Material> material = ResourceManager::GetInstance()->Find<Material>(L"LightBoltMaterial");
	weak_ptr<Texture2D> tex = ResourceManager::GetInstance()->Find<Texture2D>(L"LightBolt");
	material.lock()->SetTexture(eTextureSlot::T0, tex);

	sr->SetMesh(mesh);
	sr->SetMaterial(material);

	// ���׸��ִϸ�����
	AddComponent<GenericAnimator>();
}

void LightBolt::Update()
{
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

	// �ʱ�ȭ
	cb->SetData(&Info);
	cb->Bind(eShaderStage::PS);
}

void LightBolt::InitAnimation()
{
	Animator* animator = AddComponent<Animator>();

	weak_ptr<Texture2D> tex =
		ResourceManager::GetInstance()->Load<Texture2D>(L"LightBolt", L"LightBolt//LightBolt.png");

	animator->Create(L"LightBolt", tex, Vector2::Zero, Vector2(43.f, 98.f), Vector2::Zero, 8, 0.1f);
	animator->Play(L"LightBolt");
}

void LightBolt::SearchArriveTarget()
{
	if (mBounceCount >= mBounceMaxCount)
	{
		mBounceCount = 0;
		mCurObject = nullptr;
		Death();
		return;
	}

	Layer& layer = SceneManager::GetInstance()->GetActiveScene()->GetLayer(eLayerType::Monster);
	const vector<GameObject*> Objects = layer.GetGameObjects();

	Transform* tr = GetComponent<Transform>();
	Vector3 Pos = tr->GetPosition();


	// ���� ��ġ�� ������ ���� Ž��
	std::unordered_map<float, GameObject*> arriveList;
	for (GameObject* obj : Objects)
	{
		if (obj == nullptr)
			continue;

		if (mCurObject == nullptr)
			continue;

		if (obj == mCurObject)
			continue;

		Transform* objTr = obj->GetComponent<Transform>();
		Vector3 objPos = objTr->GetPosition();

		Vector3 Vec = Pos - objPos;

		float len = Vec.Length();
		if (len < mRadius)
			arriveList.insert(std::make_pair(len,obj));
	}


	// ������ ���͸� ã�����Ѱ�� �޸� ����
	if (arriveList.size() < 0)
	{
		Death();
		return;
	}

	mBounceCount++;

	mCurObject = arriveList.begin()->second;
	mAriiveList.insert(mCurObject);
	
	// ���� ����ġ�� ����
	float xSize = abs(Pos.y) - abs(mCurObject->GetComponent<Transform>()->GetPosition().y);
	Vector3 Size = tr->GetSize();
	Size.x = xSize;
	tr->SetSize(Size);

	// ���� �� ��ġ �� ����ġ �߰������� ��ġ�Ѵ�
	Vector3 finalPos = mCurObject->GetComponent<Transform>()->GetPosition() - Pos;
	Pos += (finalPos * 0.5f);

	tr->SetPosition(Pos);

	LinghtingRun();
}

void LightBolt::LightingOn(GameObject* object)
{
	SetCurObject(object);
	LinghtingRun();
}

void LightBolt::LinghtingRun()
{
	if (mBounceCount > mBounceMaxCount)
		return;

	GenericAnimator* generic = GetComponent<GenericAnimator>();
	if (generic->IsRunning())
		generic->Stop();

	AnimatorParam param;
	param.AnimType = eAnimType::Linear;
	param.StartValue = 0.0f;
	param.EndValue = 1.0f;
	param.DurationTime = 0.05f;

	// ���� ����
	Transform* Tr = mCurObject->GetComponent<Transform>();
	Vector3 Pos = Tr->GetPosition();
	Angle(Vector2(Pos.x,Pos.y));


	param.DurationFunc = [this](float InCurValue)
	{
		mDirection = false;
		mYScale = InCurValue;
	};

	param.CompleteFunc = [this](float InCurValue)
	{
		LightningEnd();
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
	param.DurationTime = 0.05f;

	param.DurationFunc = [this](float InCurValue)
	{
		mDirection = true;
		mYScale = InCurValue;
	};

	param.CompleteFunc = [this](float InCurValue)
	{
		SearchArriveTarget();
	};

	generic->Start(param);
}