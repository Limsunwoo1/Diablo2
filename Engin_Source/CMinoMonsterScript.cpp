#include "CMinoMonsterScript.h"
#include "CTransform.h"
#include "CMonster.h"
#include "CTime.h"
#include "CWorldManager.h"
#include "Cplayer.h"

MinoMonsterScript::MinoMonsterScript()
	: Script()
	, mCurPos(Vector2(-1.f, -1.f))
{
}

MinoMonsterScript::~MinoMonsterScript()
{
}

void MinoMonsterScript::Initalize()
{
}

void MinoMonsterScript::Update()
{
}

void MinoMonsterScript::FixedUpdate()
{
	Monster* monster = dynamic_cast<Monster*>(GetOwner());
	if (monster == nullptr)
		return;

	GameObject* player = WorldManager::GetInstance()->GetPlayer();
	if (player == nullptr)
		return;

	Transform* Tr = GetOwner()->GetComponent<Transform>();
	Vector3 mPos = Tr->GetPosition();
	Vector3 searchSize = Vector3(600.f, 600.f, 1.0f);

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



	mTime += Time::GetInstance()->DeltaTime();
	if(mTime >= 0.1f)
	{
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
		if (!mPosData.empty())
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
		
		if (abs(TargetPos.x - pos.x) < 50 && (TargetPos.y - pos.y) < 25)
		{
			mArrivePos = Vector2::Zero;
			monster->SetMonsterState(Monster::MonsterState::Attack);
			return;
		}

		pos += Vec * Time::GetInstance()->DeltaTime() * 150.f;
		tr->SetPosition(pos);
	}
}

void MinoMonsterScript::Render()
{

}

float MinoMonsterScript::GetAngle(Vector2& direction)
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

void MinoMonsterScript::ResetAStar()
{
	mArrivePos = Vector2::Zero;
	mNodePos = Vector2::Zero;

	while (!mPosData.empty())
	{
		Vector2 pos = mPosData.top();
		mPosData.pop();
	}
}