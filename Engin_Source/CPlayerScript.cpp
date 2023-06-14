#include "CPlayerScript.h"
// Object
#include "CGameObject.h"
#include "CPlayer.h"
#include "CPing.h"
#include "CObject.h"
#include "CFrozenOrb.h"
#include "CTelePort.h"
#include "CMeteor.h"

// Component
#include "CSpriteRenderer.h"
#include "CAnimator.h"
#include "CGraphicDevice_DX11.h"
#include "CAStar.h"
#include "CRenderer.h"
#include "CCamera.h"
#include "CPanel.h"

// Manager
#include "CInput.h"
#include "CTime.h"
#include "CWorldManager.h"
#include "CSceneManager.h"
#include "CResourceManager.h"
#include "CUIManager.h"

PlayerScript::PlayerScript()
	: Script()
	, mArrivePos(Vector3::Zero)
	, mPickPoint(Vector2::Zero)
	, mNode(nullptr)
	, mEndPos(Vector2(-1.f, -1.f))
{
}

PlayerScript::~PlayerScript()
{
	ClearAstar();
}

void PlayerScript::Initalize()
{
	Animator* animator = GetOwner()->GetComponent<Animator>();
}

void PlayerScript::Update()
{
	for (GameObject* obj : mRenderObj)
	{
		if (obj == nullptr)
			continue;

		obj->Update();
	}

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
	for (GameObject* obj : mRenderObj)
	{
		if (obj == nullptr)
			continue;

		obj->FixedUpdate();
	}

	Player* player = dynamic_cast<Player*>(GetOwner());

	if (player == nullptr)
		return;

	Transform* tr = player->GetComponent<Transform>();
	Vector3 pos = tr->GetPosition();

	float speed = 3.f;
	if (Input::GetInstance()->GetKeyDown(eKeyCode::I))
	{
		Panel* inventory = UIManager::GetInstance()->GetUiInstance<Panel>(L"mainInventory");
		bool able = inventory->GetIsAble();

		if (able)
		{
			inventory->UnActive();
		}
		else
		{
			inventory->OnActive();
		}
	}

	if (Input::GetInstance()->GetKeyDown(eKeyCode::A))
	{
		if (player->GetState() == Player::PlayerState::Idle
			|| player->GetState() == Player::PlayerState::Move)
		{
			player->SetState(Player::PlayerState::Attack);

			SetPlayerDirection();
			ResetAStar();
			return;
		}
	}
	else if (Input::GetInstance()->GetKeyDown(eKeyCode::S))
	{
		if (player->GetState() == Player::PlayerState::Idle
			|| player->GetState() == Player::PlayerState::Move)
		{
			player->SetState(Player::PlayerState::Skil);

			FrozenOrb* orb = Object::Instantiate<FrozenOrb>(eLayerType::PlayerSKil, true);
			Player* player = dynamic_cast<Player*>(GetOwner());
			orb->SetOwner(player);

			Vector3 OwnerPos = GetOwner()->GetComponent<Transform>()->GetPosition();

			Vector2 direction = (Input::GetInstance()->GetMouseWorldPos() - pos);
			orb->SetDirection(direction);

			Transform* tr = orb->GetComponent<Transform>();
			direction.Normalize();
			Vector3 SpawnPos = Vector3(direction.x, direction.y, 1.0f) * 1.f;
			OwnerPos += SpawnPos;
			tr->SetPosition(OwnerPos);

			SetPlayerDirection();
			ResetAStar();
			return;
		}
	}
	else if (Input::GetInstance()->GetKeyDown(eKeyCode::D))
	{
		return;
		if (player->GetState() == Player::PlayerState::Idle
			|| player->GetState() == Player::PlayerState::Move)
		{
			player->SetState(Player::PlayerState::Skil);

			Vector2 direction = (Input::GetInstance()->GetMouseWorldPos() - pos);
			direction.Normalize();
			Vector2 EndPos = direction * 2;

			EndPos.x += pos.x;
			EndPos.y += pos.y;

			TelePort* teleport = Object::Instantiate<TelePort>(eLayerType::Effect, true);

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

			SetPlayerDirection();
			ResetAStar();
			return;
		}
	}
	else if (Input::GetInstance()->GetKeyDown(eKeyCode::F))
	{
		if (player->GetState() == Player::PlayerState::Idle
			|| player->GetState() == Player::PlayerState::Move)
		{
			player->SetState(Player::PlayerState::Skil);

			Meteor* meteor = Object::Instantiate<Meteor>(eLayerType::PlayerSKil, true);
			Transform* MeteorTr = meteor->GetComponent<Transform>();

			Player* player = dynamic_cast<Player*>(GetOwner());
			meteor->SetOwner(player);


			Vector2 mousePos = Input::GetInstance()->GetMouseWorldPos();
			Vector3 pinPos = Vector3(mousePos.x, mousePos.y, 1.0f);
			meteor->SetPinPos(pinPos);

			Vector3 meteorPos = tr->GetPosition();
			// 화면밖렌더
			meteorPos.y += 1000.f;

			MeteorTr->SetPosition(Vector3(mousePos.x, meteorPos.y, 1.0f));

			SetPlayerDirection();
			ResetAStar();
			return;
		}
	}

	if (Input::GetInstance()->GetKeyDown(eKeyCode::R))
	{
		player->ChangeRunMode();
	}

	if (Input::GetInstance()->GetKeyPress(eKeyCode::RBTN))
	{
		Player* player = dynamic_cast<Player*>(GetOwner());
		if (player->GetState() != Player::PlayerState::Idle &&
			player->GetState() != Player::PlayerState::Move)
			return;
		//a*
		AStar* astar = GetOwner()->GetComponent<AStar>();
		AStar::Node node = {};
		AStar::Vec end = {};

		if (!mbInput)
		{
			Vector2 mousePos = Input::GetInstance()->GetMouseWorldPos(true);
			auto Idx = WorldManager::GetInstance()->GetTileIndex(mousePos);
			if (Idx.first >= 0 && Idx.second >= 0)
			{
				astar->OnA_Star(Idx, mousePos);
				ResetAStar();
			}

			if (WorldManager::GetInstance()->SetPath(node.Pos.x, node.Pos.y, end.x, end.y))
			{
				if (astar->OnA_Star(Idx, mousePos))
				{
					mbInput = true;
					/*Vector2 pos = WorldManager::GetInstance()->GetPlayerIndex();
					WorldManager::GetInstance()->SetObstacle(pos.x, pos.y);

					Vector3 trPos = GetOwner()->GetComponent<Transform>()->GetPosition();

					WorldManager::GetInstance()->SetPlayerIndex(trPos.x, trPos.y);

					mPickPoint = Vector2::Zero;
					mEndPos = index;*/
				}
			}
			///////////////////////////////////////////////////////////////////////////////////
		}
	}

	if (mNodePos == Vector2::Zero && mPickPoint == Vector2::Zero)
	{
		if (!mPosData.empty())
		{
			mNodePos = mPosData.top();
			mPosData.pop();

			mPickPoint = mNodePos;

			Vector3 driection = Vector3(mPickPoint.x, mPickPoint.y, 1.0f);
			SetPlayerDirection(driection);
		}
	}

	if (mPickPoint != Vector2::Zero)
	{
		Vector3 PosUnder = pos;
		PosUnder.y -= tr->GetSize().y * 0.25f;

		Vector3 vec = mPickPoint - PosUnder;

		if (fabs(vec.x) < 5.0f && fabs(vec.y) < 5.0f)
		{
			mPickPoint = Vector2::Zero;
			mNodePos = Vector2::Zero;

			AStar* astar = GetOwner()->GetComponent<AStar>();
			astar->PopNode();

			player->SetState(Player::PlayerState::Idle);
			return;
		}

		/*Vector3 driection = Vector3(mPickPoint.x, mPickPoint.y, 1.0f);
		SetPlayerDirection(driection);*/

		vec.Normalize();
		player->SetState(Player::PlayerState::Move);

		pos += vec * Time::GetInstance()->DeltaTime() * player->GetRunSpeed() * 200;
	}

	if (player->GetState() == Player::PlayerState::Idle
		|| player->GetState() == Player::PlayerState::Move)
	{
		if (player->GetRunMode() == true)
		{
			// 이동시에만 게이지 소모
			Vector3 curPos = GetOwner()->GetComponent<Transform>()->GetPosition();
			if (pos == curPos)
				return;

			float runTime = player->GetRunTime();
			runTime -= Time::GetInstance()->DeltaTime();

			if (runTime < 0.0f)
			{
				player->ChangeRunMode();
				runTime = 0.0f;
			}

			player->SetRunTime(runTime);
		}
		else
		{
			float runTime = player->GetRunTime();
			runTime += Time::GetInstance()->DeltaTime() * 2;

			if (runTime > player->GetMaxRunTime())
				runTime = player->GetMaxRunTime();

			player->SetRunTime(runTime);
		}

		tr->SetPosition(pos);
	}
}

void PlayerScript::Render()
{
	for (GameObject* obj : mRenderObj)
	{
		if (obj == nullptr)
			continue;

		obj->Render();
	}
}

void PlayerScript::ResetAStar()
{
	AStar* astar = GetOwner()->GetComponent<AStar>();
	astar->ClearNode();
	mPickPoint = Vector2::Zero;
	mNodePos = Vector2::Zero;

	while (!mPosData.empty())
	{
		mPosData.pop();
	}

	mNode = nullptr;
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

void PlayerScript::SetPlayerDirection()
{
	Player* player = dynamic_cast<Player*>(GetOwner());
	Vector3 pos = player->GetComponent<Transform>()->GetPosition();
	Vector2 mousePos = Input::GetInstance()->GetMouseWorldPos();

	Vector3 vec = mousePos - pos;

	vec.Normalize();

	// SetPlayerIndex

	float angle = GetAngle(mousePos);

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
}

void PlayerScript::SetPlayerDirection(Vector3& Direction)
{
	Player* player = dynamic_cast<Player*>(GetOwner());
	Vector3 pos = player->GetComponent<Transform>()->GetPosition();

	Vector3 vec = Direction - pos;

	vec.Normalize();

	// SetPlayerIndex

	float angle = GetAngle(Vector2(Direction.x, Direction.y));

	if (vec.x <= 0.0f)
	{
		if (angle >= 0.0f && angle < 22.5f)
		{
			player->PlayerDirection(0);
		}
		else if (angle >= 22.5f && angle < 50.f)
		{
			player->PlayerDirection(2);
		}
		else if (angle >= 50.f && angle < 75.5f)
		{
			player->PlayerDirection(2);
		}
		else if (angle >= 75.5f && angle < 90.f)
		{
			player->PlayerDirection(4);
		}
		else if (angle >= 90.f && angle < 115.f)
		{
			player->PlayerDirection(4);
		}
		else if (angle >= 115.f && angle < 140.f)
		{
			player->PlayerDirection(6);
		}
		else if (angle >= 140.f && angle < 165.5f)
		{
			player->PlayerDirection(6);
		}
		else
		{
			player->PlayerDirection(8);
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
			player->PlayerDirection(10);
		}
		else if (angle >= 50.f && angle < 75.5f)
		{
			player->PlayerDirection(10);
		}
		else if (angle >= 75.5f && angle < 90.f)
		{
			player->PlayerDirection(12);
		}
		else if (angle >= 90.f && angle < 115.f)
		{
			player->PlayerDirection(12);
		}
		else if (angle >= 115.f && angle < 140.f)
		{
			player->PlayerDirection(14);
		}
		else if (angle >= 140.f && angle < 165.5f)
		{
			player->PlayerDirection(14);
		}
		else
		{
			player->PlayerDirection(0);
		}
	}
}

void PlayerScript::AddRenderAStar()
{
	ClearAstar();

	AStar* astar = GetOwner()->GetComponent< AStar>();
	mPosData = astar->GetPosData();
}

void PlayerScript::ClearAstar()
{
	/*for (GameObject* obj : mRenderObj)
	{
		if (obj == nullptr)
			continue;

		delete obj;
		obj = nullptr;
	}
	mRenderObj.clear();*/
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