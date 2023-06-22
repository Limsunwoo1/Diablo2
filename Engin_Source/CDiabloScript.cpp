#include "CDiabloScript.h"
#include "CDiabloMonster.h"
#include "CTransform.h"
#include "CMonster.h"
#include "CTime.h"
#include "CWorldManager.h"
#include "Cplayer.h"
#include "CDiabloMonster.h"
#include "CObject.h"
#include "CAnimator.h"

DiabloScript::DiabloScript()
	: Script()
	, mCurPos(Vector2(-1.f, -1.f))
{
}

DiabloScript::~DiabloScript()
{
}

void DiabloScript::Initalize()
{
}

void DiabloScript::Update()
{
}

void DiabloScript::FixedUpdate()
{
	Monster* owner = dynamic_cast<Monster*>(GetOwner());
	if (owner == nullptr)
		return;

	DiabloMonster* monster = dynamic_cast<DiabloMonster*>(owner);

	if (monster == nullptr)
		return;

	if (monster->GetMonsterState() == Monster::MonsterState::Dead)
		return;

	float Delta = monster->GetCurDeltaTime();
	GameObject* player = WorldManager::GetInstance()->GetPlayer();
	if (player == nullptr)
		return;

	Transform* Tr = GetOwner()->GetComponent<Transform>();
	Vector3 mPos = Tr->GetPosition();
	Vector3 searchSize = Vector3(1000.f, 700.f, 1.0f);

	Transform* playertr = player->GetComponent<Transform>();
	Vector3 PlayerPos = playertr->GetPosition();

	if (monster->GetTarget() == nullptr)
	{
		if (PlayerPos.x < mPos.x - (searchSize.x * 0.5f) || PlayerPos.x > mPos.x + (searchSize.x * 0.5f))
			return;

		if (PlayerPos.y < mPos.y - (searchSize.y * 0.5f) || PlayerPos.y > mPos.y + (searchSize.y * 0.5f))
			return;

		monster->SetTarget(player);
	}


	if (monster->GetSkilCurTime() > monster->GetSkilCoolTime() &&
		monster->GetComponent<Animator>()->GetPlayAnimation()->AnimationName().find(L"Attack") == std::wstring::npos)
	{
		Transform* playerTr = player->GetComponent<Transform>();
		Vector3 playerPos = playerTr->GetPosition();

		Transform* OwnerTr = monster->GetComponent<Transform>();
		Vector3 OwnerPos = OwnerTr->GetPosition();

		Vector3 diff = playerPos - OwnerPos;
		float len = diff.Length();

		if (len < 500.f)
		{
			ResetAStar();
			mArrivePos = Vector2::Zero;
			monster->SetMonsterState(Monster::MonsterState::Attack);
			return;
		}
	}


	mTime += Time::GetInstance()->DeltaTime();
	if (mTime >= mMaxTime)
	{
		mMaxTime = 0.1f;

		AStar* astar = GetOwner()->GetComponent<AStar>();
		if (astar == nullptr)
			return;

		Transform* followTr = monster->GetTarget()->GetComponent<Transform>();
		Vector3 followPos = followTr->GetPosition();
		Vector2 fPos = Vector2(followPos.x, followPos.y);

		auto Idx = WorldManager::GetInstance()->GetTileIndex(fPos);
		if (Idx.first >= 0 && Idx.second >= 0)
		{
			if (astar->OnA_Star(Idx, fPos))
			{
				ResetAStar();
				mTime = 0.0f;
			}
		}
	}

	if (mNodePos == Vector2::Zero && mArrivePos == Vector2::Zero)
	{
		if (!mPosData.empty() && monster->GetMonsterState() != Monster::MonsterState::Attack)
		{
			mNodePos = mPosData.top();
			mPosData.pop();

			mArrivePos = mNodePos;

			Vector3 driection = Vector3(mArrivePos.x, mArrivePos.y, 1.0f);
			Vector2 V2Direction = Vector2(driection.x, driection.y);

			float angle = GetAngle(V2Direction);

			Transform* Tr = GetOwner()->GetComponent<Transform>();
			Vector3 monsterPos = Tr->GetPosition();
			Vector3 Vec = monsterPos - mArrivePos;
			Vec.Normalize();

			if (Vec.x < 0.f)
			{
				if (angle >= 0 && angle < 45.f)
				{
					monster->MonsterDirection(0);
				}
				else if (angle >= 45.f && angle < 90.f)
				{
					monster->MonsterDirection(1);
				}
				else if (angle >= 90.f && angle < 135.f)
				{
					monster->MonsterDirection(2);
				}
				else if (angle >= 135.f && angle < 180.f)
				{
					monster->MonsterDirection(3);
				}
			}
			else
			{
				if (angle >= 0 && angle < 45.f)
				{
					monster->MonsterDirection(4);
				}
				else if (angle >= 45.f && angle < 90.f)
				{
					monster->MonsterDirection(5);
				}
				else if (angle >= 90.f && angle < 135.f)
				{
					monster->MonsterDirection(6);
				}
				else if (angle >= 135.f && angle < 180.f)
				{
					monster->MonsterDirection(7);
				}
			}

			monster->MonsterDirection(angle);

			monster->SetMonsterState(Monster::MonsterState::Move);
		}
	}

	if (monster->GetTarget() && mArrivePos != Vector2::Zero)
	{
		if (monster->GetMonsterState() == Monster::MonsterState::Attack)
			return;

		Transform* tr = monster->GetComponent<Transform>();
		Vector3 pos = tr->GetPosition();

		Transform* TragetTr = monster->GetTarget()->GetComponent<Transform>();
		Vector3 TargetPos = TragetTr->GetPosition();

		Vector3 Vec = mArrivePos - pos;

		Vec.Normalize();
		Vec.z = 0.0f;

		Vector3 lenght = TargetPos - pos;
		float len = lenght.Length();

		if (len <= 100.f)
		{
			mArrivePos = Vector2::Zero;
			monster->SetMonsterState(Monster::MonsterState::Attack);
			return;
		}

		pos += Vec * Delta * 350.f;
		tr->SetPosition(pos);
	}
}

void DiabloScript::Render()
{
}

float DiabloScript::GetAngle(Vector2& direction)
{
	Transform* tr = GetOwner()->GetComponent<Transform>();

	Vector3 pos = tr->GetPosition();
	Vector2 vec = direction - Vector2(pos.x, pos.y);


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

	return angle;
}

void DiabloScript::ResetAStar()
{
	mArrivePos = Vector2::Zero;
	mNodePos = Vector2::Zero;

	while (!mPosData.empty())
	{
		Vector2 pos = mPosData.top();
		mPosData.pop();
	}
}