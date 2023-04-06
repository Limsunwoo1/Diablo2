#include "CPlayerScript.h"
#include "CGameObject.h"
#include "CInput.h"
#include "CTime.h"
#include "CAnimator.h"
#include "CPlayer.h"
#include "CGraphicDevice_DX11.h"
#include "CAStart.h"
#include "CWorldManager.h"
#include "CRenderer.h"
#include "CCamera.h"
#include "CPing.h"
#include "CObject.h"
#include "CSceneManager.h"

PlayerScript::PlayerScript()
	: Script()
	, mArrivePos(Vector3::Zero)
	, mbRun(false)
	, mPickPoint(Vector2::Zero)
	, mNode(nullptr)
{
}

PlayerScript::~PlayerScript()
{
}

void PlayerScript::Initalize()
{
	Animator* animator = GetOwner()->GetComponent<Animator>();
	//animator->GetStartEvent(L"Idle") = bind(&PlayerScript::Start, this);
	//animator->GetCompleteEvent(L"Idle") = bind(&PlayerScript::Start, this);
	//animator->GetEndEvent(L"Idle") = bind(&PlayerScript::Start, this);
	//animator->GetEvent(L"Idle", 2) = bind(&PlayerScript::Start, this);
}

void PlayerScript::Update()
{

	Player* player = dynamic_cast<Player*>(GetOwner());

	if (player == nullptr)
		return;

	Transform* tr = player->GetComponent<Transform>();
	Vector3 pos = tr->GetPosition();

	cout << "X    " << pos.x << "    Y    " << pos.y << endl;

	float speed = 2.f;

	if (Input::GetInstance()->GetKeyPress(eKeyCode::A))
	{
		if (player->GetState() == Player::State::Idle
			|| player->GetState() == Player::State::Move)
			player->SetState(Player::State::Attack);
	}
	else if (Input::GetInstance()->GetKeyPress(eKeyCode::S))
	{
		if (player->GetState() == Player::State::Idle
			|| player->GetState() == Player::State::Move)
			player->SetState(Player::State::Skil);
	}


	if (Input::GetInstance()->GetKeyUp(eKeyCode::RBTN))
	{
		//a*
		AStar* astar = GetOwner()->GetComponent<AStar>();
		AStar::Node node = {};
		AStar::Vec end = {};

		if (!astar->IsRunning())
		{
			Vector2 index = Input::GetInstance()->GetMouseWorldPos();

			Vector2 Wpos = WorldManager::GetInstance()->GetPlayerIndex();
			Vector2 Wend = WorldManager::GetInstance()->GetEndIndex();

			Vector2 playerIndex = WorldManager::GetInstance()->GetPlayerIndex();
			node.Pos.x = (UINT)playerIndex.x;
			node.Pos.y = (UINT)playerIndex.y;

			end.x = index.x;
			end.y = index.y;

			UINT max = WorldManager::GetInstance()->GetScale();
			node.Id = (node.Pos.y * max) + (node.Pos.x % max);

			if (WorldManager::GetInstance()->SetPath(node.Pos.x, node.Pos.y, end.x, end.y))
			{
				if (astar->OnA_Star(node, node.Pos, end))
					mPickPoint = Vector2::Zero;
			}

			Scene* scene = SceneManager::GetInstance()->GetActiveScene();
			if (scene)
			{
				Ping* ping = Object::Instantiate<Ping>(eLayerType::Effect, scene);

				Transform* tr = ping->GetComponent<Transform>();
				Vector3 posVec = Vector3(index.x, index.y, 1.0f);
				tr->SetPosition(posVec);

				//ping->SetPos(index);
			}
			///////////////////////////////////////////////////////////////////////////////////
		}
	}


	if (Input::GetInstance()->GetKeyDown(eKeyCode::RBTN))
	{
		//mPickPoint = Input::GetInstance()->GetMouseWorldPos();

	}
	if (mNode == nullptr && mPickPoint == Vector2::Zero)
	{
		AStar* astar = GetOwner()->GetComponent<AStar>();
		mNode = astar->GetNextNode();
		if (mNode != nullptr)
		{
			// lefttop ÁÂÇ¥¸¦ 32¸¦ ´õÇØ Å¸ÀÏ Áß¾Ó¿¡ ÁÂÇ¥¸¦ ¼³Á¤
			mPickPoint = Vector2((mNode->Pos.x), (mNode->Pos.y));
			if (WorldManager::GetInstance()->GetEndIndex() == mPickPoint)
			{
				WorldManager::GetInstance()->SetPlayerIndex(mNode->Pos.x, mNode->Pos.y);
			}
		}
	}

	if (mPickPoint != Vector2::Zero)
	{

		Vector3 vec = mPickPoint - pos;

		if (fabs(vec.x) < 0.005f && fabs(vec.y) < 0.005f)
		{
			Vector2 pos = WorldManager::GetInstance()->GetPlayerIndex();
			WorldManager::GetInstance()->SetZero(pos.x, pos.y);

			mPickPoint = Vector2::Zero;

			player->SetState(Player::State::Idle);
			return;
		}

		vec.Normalize();

		// SetPlayerIndex
		{
			float angle = GetAngle(mPickPoint);

			if (vec.x <= 0.0f)
			{
				if (angle >= 0.0f && angle < 22.5f)
				{
					player->PlayerDirection(0);
				}
				else if (angle >= 22.5f && angle < 50.f)
				{
					player->PlayerDirection(1);
				}
				else if (angle >= 50.f && angle < 75.5f)
				{
					player->PlayerDirection(2);
				}
				else if (angle >= 75.5f && angle < 90.f)
				{
					player->PlayerDirection(3);
				}
				else if (angle >= 90.f && angle < 115.f)
				{
					player->PlayerDirection(4);
				}
				else if (angle >= 115.f && angle < 140.f)
				{
					player->PlayerDirection(5);
				}
				else if (angle >= 140.f && angle < 165.5f)
				{
					player->PlayerDirection(6);
				}
				else
				{
					player->PlayerDirection(7);
				}
			}
			else
			{
				if (angle >= 0.0f && angle < 22.5f)
				{
					player->PlayerDirection(8);
				}
				else if (angle >= 22.5f && angle < 50.f)
				{
					player->PlayerDirection(9);
				}
				else if (angle >= 50.f && angle < 75.5f)
				{
					player->PlayerDirection(10);
				}
				else if (angle >= 75.5f && angle < 90.f)
				{
					player->PlayerDirection(11);
				}
				else if (angle >= 90.f && angle < 115.f)
				{
					player->PlayerDirection(12);
				}
				else if (angle >= 115.f && angle < 140.f)
				{
					player->PlayerDirection(13);
				}
				else if (angle >= 140.f && angle < 165.5f)
				{
					player->PlayerDirection(14);
				}
				else
				{
					player->PlayerDirection(15);
				}
			}
			player->SetState(Player::State::Move);
		}

		pos += vec * Time::GetInstance()->DeltaTime() * speed;

		mNode = nullptr;
	}

	if (player->GetState() == Player::State::Idle
		|| player->GetState() == Player::State::Move)
		tr->SetPosition(pos);
}

void PlayerScript::FixedUpdate()
{
}

void PlayerScript::Render()
{
}

void PlayerScript::Start()
{
}

void PlayerScript::Action()
{
}

void PlayerScript::End()
{
}

void PlayerScript::Idle()
{

}

void PlayerScript::Move()
{

}

float PlayerScript::GetAngle(Vector2 point)
{
	Transform* tr = GetOwner()->GetComponent<Transform>();

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

	return angle;
}