#include "CBoltBase.h"
#include "CTransform.h"
#include "CTime.h"
#include "CAnimator.h"
#include "CBaseRenderer.h"
#include "CAudioSource.h"
#include "CSceneManager.h"

BoltBase::BoltBase()
	: mDirection(0.0f)
	, mSpeed(600.f)
	, mbRun(false)
{
	SetDamege(20.f);
}

BoltBase::~BoltBase()
{
}

void BoltBase::Update()
{
	if (mbRun == false)
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

		mbRun = true;
		renderer->SetRenderStop(false);
	}

		

	Transform* tr = GetComponent<Transform>();
	Vector3 Pos = tr->GetPosition();

	// ���ҽ� ������ �Ʒ������� �����ֱ⿡ up �������Ͱ� �ݴ����
	Pos += mSpeed * -tr->Up() *  Time::GetInstance()->DeltaTime();

	tr->SetPosition(Pos);

	Skil::Update();
}

void BoltBase::SetDirection(float angle)
{
	mDirection = angle;

	Transform* tr = GetComponent<Transform>();
	Vector3 Rotation = tr->GetRotation();
	Rotation.z = angle;

	tr->SetRotation(Rotation);
}

float BoltBase::Angle(Vector2 point)
{
	Transform* tr = GetComponent<Transform>();

	Vector3 pos = tr->GetPosition();
	Vector2 vec = point - Vector2(pos.x, pos.y);

	Vector2 Vec1 = Vector2(0.0f, 0.0f);

	if (vec.x <= 0.f)
		Vec1 = Vector2(0.0f, -1.0f);
	else
		Vec1 = Vector2(0.0f, 1.0f);

	Vec1.Normalize();
	vec.Normalize();

	float that = Vec1.Dot(vec);
	float radian = acos(that);
	float angle = XMConvertToDegrees(radian);

	mDirection = angle;

	if (vec.x < 0.f)
	{
		// 6�� ���� ���ҽ� ���ؿ� ���� ������ ������ ����
		mDirection = -mDirection;
	}
	else
	{
		// ��� ���� 
		mDirection = 180.f - mDirection;
	}

	SetDirection(mDirection);

	return angle;
}

void BoltBase::HitSkil(GameObject* obj)
{

}