#include "CMonster.h"
#include "CTime.h"
#include "CAnimator.h"

Monster::Monster()
	: GameObject()
	, mMonsterState(MonsterState::Idle)
	, mMaxHP(100.f)
	, mHP(100.f)
	, mIndex(4)
	, mDotDamageCoolTime(0.0f)
	, mDeltaTime(0.0f)
	, mDirection{}
{
	mDirection[mIndex] = 1;
}

Monster::~Monster()
{
}

void Monster::Initalize()
{
}

void Monster::Update()
{
	float time = Time::GetInstance()->DeltaTime();
	SetCurDeltaTime(time);
	StatusEffect();

	// ������ ���� ���¿� ���� ���� �Ǵ� ����� ������ enum ��
	// ��) ���� �����̻��Ͻ� ���� Ǫ�������� ����
	Animator* animator = GetComponent<Animator>();
	animator->SetElementType((UINT)mMonsterState);

	Run();
	GameObject::Update();
}

void Monster::FixedUpdate()
{
	GameObject::FixedUpdate();
}

void Monster::Render()
{
	GameObject::Render();
}

void Monster::StatusEffect()
{
	float time = Time::GetInstance()->DeltaTime();
	if (mMonsterState == MonsterState::HitFrozen)
	{
		// ���� �Ӽ��϶� ��� �ൿ������ �ι� ������ �̵�
		float intime = 0.0f;
		Animation* animation = GetComponent<Animator>()->GetPlayAnimation();
		if (animation)
		{
			animation->SetTimeControl(true);
			intime = animation->GetTime();
			intime += time * 0.5f;
			animation->SetTime(intime);
		}
	}
	else if (mMonsterState == MonsterState::HitFire)
	{
		if (mDotDamageCoolTime >= 0.5f)
		{
			mDotDamageCoolTime -= 0.5f;
			int hp = GetHP();
			hp -= 5;
			SetHP(hp);
		}
	}
}

void Monster::Run()
{
	switch (mMonsterState)
	{
	case Monster::MonsterState::Idle:		Idle();			break;
	case Monster::MonsterState::Move:		Move();			break;
	case Monster::MonsterState::Attack:		Attack();		break;
	case Monster::MonsterState::Hit:		Hit();			break;
	case Monster::MonsterState::Dead:		Dead();			break;
	case Monster::MonsterState::HitFire:	HitFire();		break;
	case Monster::MonsterState::HitFrozen:	HitFrozen();	break;
	case Monster::MonsterState::HitLight:	HitLight();		break;
	default:												break;
	}
}

bool Monster::MonsterDirection(int index)
{
	if (index >= mDirection.size())
		return false;

	mDirection.reset();
	mDirection[index] = 1;
	mIndex = index;

	return true;
}

UINT Monster::GetDirection()
{
	return mIndex;
}