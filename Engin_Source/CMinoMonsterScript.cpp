#include "CMinoMonsterScript.h"
#include "CTransform.h"
#include "CMonster.h"
#include "CTime.h"

MinoMonsterScript::MinoMonsterScript()
	: Script()
	, mCurPos(Vector2(-1.f, -1.f))
	, mNode(nullptr)
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

	if (monster->GetTarget() == nullptr)
		return;

	AStar* astar = monster->GetComponent<AStar>();
	if (mNode == nullptr &&
		(mCurPos.x < 0 && mCurPos.y < 0))
	{
		mNode = astar->GetNextNode();
		if (mNode)
		{
			mCurPos.x = mNode->Pos.x;
			mCurPos.y = mNode->Pos.y;
		}
	}

	Transform* tr = monster->GetComponent<Transform>();
	Vector3 pos = tr->GetPosition();

	if (mCurPos.x >= 0 && mCurPos.y >= 0)
	{
		Vector2 vec = Vector2(mCurPos.x - pos.x, mCurPos.y  - pos.y);
		if (fabs(vec.x) < 0.05f && fabs(vec.y) < 0.05f)
		{
			mCurPos = Vector2(-1.f, -1.f);
			mNode = nullptr;

			return;
		}

		float angle = GetAngle(mCurPos);
		if (vec.x > 0.f)
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

		//monster->MonsterDirection(angle);

		vec.Normalize();
		monster->SetMonsterState(Monster::MonsterState::Move);
		pos += Vector3(vec.x, vec.y, 0.0f) * monster->GetCurDeltaTime() * 2;
		tr->SetPosition(pos);
	}

	mTime += Time::GetInstance()->DeltaTime();
	if(mTime >= 2.0f)
	{
		mTime -= 2.0f;
		AStar* astar = GetOwner()->GetComponent<AStar>();
		Transform* tr = GetOwner()->GetComponent<Transform>();

		AStar::Node node;
		Vector3 pos = tr->GetPosition();
		Vector3 targetPos = monster->GetTarget()->GetComponent<Transform>()->GetPosition();

		node.Pos.x = pos.x;
		node.Pos.y = pos.y;

		astar->OnA_Star(node, (int)pos.x, (int)pos.y, (int)targetPos.x, (int)targetPos.y);
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