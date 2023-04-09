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
#include "CFrozenOrb.h"
#include "CTelePort.h"

PlayerScript::PlayerScript()
	: Script()
	, mArrivePos(Vector3::Zero)
	, mbInput(false)
	, mPickPoint(Vector2::Zero)
	, mNode(nullptr)
	, mInputDelay(0.0f)
	, mEndPos(Vector2(-1.f, -1.f))
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
	if (mInputDelay >= 0.1f)
	{
		mInputDelay -= 0.1f;
		mbInput = false;
		return;
	}

	if (mbInput)
	{
		mInputDelay += Time::GetInstance()->DeltaTime();
	}
	
}

void PlayerScript::FixedUpdate()
{
	Player* player = dynamic_cast<Player*>(GetOwner());

	if (player == nullptr)
		return;

	Transform* tr = player->GetComponent<Transform>();
	Vector3 pos = tr->GetPosition();

	float speed = 2.f;

	if (Input::GetInstance()->GetKeyDown(eKeyCode::A))
	{
		if (player->GetState() == Player::State::Idle
			|| player->GetState() == Player::State::Move)
		{
			player->SetState(Player::State::Attack);
			return;
		}
	}
	else if (Input::GetInstance()->GetKeyDown(eKeyCode::S))
	{
		if (player->GetState() == Player::State::Idle
			|| player->GetState() == Player::State::Move)
		{
			player->SetState(Player::State::Skil);

			FrozenOrb* orb = Object::Instantiate<FrozenOrb>(eLayerType::PlayerSKil, true);
			Vector2 direction = (Input::GetInstance()->GetMouseWorldPos() - pos);
			orb->SetDirection(direction);

			Transform* tr = orb->GetComponent<Transform>();
			tr->SetPosition(pos);

			orb->OnOrb();

			return;
		}
	}
	else if (Input::GetInstance()->GetKeyDown(eKeyCode::D))
	{
		if (player->GetState() == Player::State::Idle
			|| player->GetState() == Player::State::Move)
		{
			player->SetState(Player::State::Skil);

			Vector2 direction = (Input::GetInstance()->GetMouseWorldPos() - pos);
			direction.Normalize();
			Vector2 EndPos = direction * 2;

			EndPos.x += pos.x;
			EndPos.y += pos.y;

			TelePort* teleport = Object::Instantiate<TelePort>(eLayerType::PlayerSKil, true);

			Player* player = dynamic_cast<Player*>(GetOwner());
			if (player)
				teleport->SetOwner(player);

			if (WorldManager::GetInstance()->GetTileNum(EndPos.x, EndPos.y) == 3)
			{
				return;
			}

			Vector2 playerIndex = WorldManager::GetInstance()->GetPlayerIndex();

			pos = Vector3(EndPos.x, EndPos.y, pos.z);
			teleport->SetMovePos(pos);

			AStar* astar = GetOwner()->GetComponent<AStar>();
			astar->ClearNode();
			mPickPoint = Vector2::Zero;
			mNode = nullptr;

			return;
		}
	}


	if (Input::GetInstance()->GetKeyDown(eKeyCode::RBTN))
	{
		//a*
		AStar* astar = GetOwner()->GetComponent<AStar>();
		AStar::Node node = {};
		AStar::Vec end = {};

		if (!mbInput)
		{
			Vector2 index = Input::GetInstance()->GetMouseWorldPos();

			// 플레이어 발끝에 위치하기위해
			// 약간의 y 값을 증가시킴
			//index.y += 0.7f;

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
				{
					Vector2 pos = WorldManager::GetInstance()->GetPlayerIndex();
					WorldManager::GetInstance()->SetObstacle(pos.x, pos.y);

					Vector3 trPos = GetOwner()->GetComponent<Transform>()->GetPosition();

					WorldManager::GetInstance()->SetPlayerIndex(trPos.x, trPos.y);

					mPickPoint = Vector2::Zero;
					mEndPos = index;
				}

				Scene* scene = SceneManager::GetInstance()->GetActiveScene();
				if (scene != nullptr)
				{
					Ping* ping = Object::Instantiate<Ping>(eLayerType::Effect, true);

					Transform* tr = ping->GetComponent<Transform>();

					Vector3 posVec = Vector3(index.x, index.y, 1.0f);
					tr->SetPosition(posVec);

					mbInput = true;
				}
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
			mPickPoint = Vector2((mNode->Pos.x), (mNode->Pos.y));
			if (WorldManager::GetInstance()->GetEndIndex() == mPickPoint)
			{
				WorldManager::GetInstance()->SetPlayerIndex(mNode->Pos.x, mNode->Pos.y);
			}

			if (astar->GetNodeEmpyt())
			{
				WorldManager::GetInstance()->SetZero(mNode->Pos.x, mNode->Pos.y);

				mPickPoint = mEndPos;
				WorldManager::GetInstance()->SetPlayerIndex(mPickPoint.x, mPickPoint.y);

				mEndPos = Vector2(-1.f, -1.f);
			}
		}
	}

	if (mPickPoint != Vector2::Zero)
	{
		Vector3 vec = mPickPoint - pos;

		if (fabs(vec.x) < 0.05f && fabs(vec.y) < 0.05f)
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

	if (Input::GetInstance()->GetKeyPress(eKeyCode::R))
	{
		Vector3 rotation = tr->GetRotation();
		rotation.z += 0.5f;

		tr->SetRotation(rotation);
	}

	if (Input::GetInstance()->GetKeyPress(eKeyCode::G))
	{
		pos += tr->Right() * Time::GetInstance()->DeltaTime() * 2.0f;
		tr->SetPosition(pos);
	}
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