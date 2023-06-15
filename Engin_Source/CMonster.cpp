#include "CMonster.h"
#include "CTime.h"
#include "CAnimator.h"

Monster::Monster()
	: GameObject()
	, mMonsterState(MonsterState::Idle)
	, mMonsterStatusEffect(eElementType::None)
	, mMaxHP(100.f)
	, mHP(100.f)
	, mIndex(4)
	, mDotDamageCoolTime(0.0f)
	, mDeltaTime(0.0f)
	, mReset(false)
	, mDirection{}
	, mSpawnPos(-1.f, -1.f)
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
	animator->SetElementType((UINT)mMonsterStatusEffect);

	if(mTarget == nullptr)
		mReset = true;

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
	if (mMonsterStatusEffect == eElementType::HitFrozen)
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
			SetCurDeltaTime(time * 0.5f);
		}
	}
	else if (mMonsterStatusEffect == eElementType::HitFire)
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
	case Monster::MonsterState::Idle:		idle();			break;
	case Monster::MonsterState::Move:		move();			break;
	case Monster::MonsterState::Attack:		attack();		break;
	case Monster::MonsterState::Hit:		hit();			break;
	case Monster::MonsterState::Dead:		monsterDead();	break;
	default:												break;
	}

	switch (mMonsterStatusEffect)
	{
	case eElementType::HitFire:		hitFire();				break;
	case eElementType::HitFrozen:	hitFrozen();			break;
	case eElementType::HitLight:		hitLight();				break;
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