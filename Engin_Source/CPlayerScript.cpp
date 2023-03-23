#include "CPlayerScript.h"
#include "CTransform.h"
#include "CGameObject.h"
#include "CInput.h"
#include "CTime.h"
#include "CAnimator.h"
#include "CPlayer.h"
#include "CGraphicDevice_DX11.h"

PlayerScript::PlayerScript()
	: Script()
	, mArrivePos(Vector3::Zero)
	, mbRun(false)
	, mPickPoint(Vector2::Zero)
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
	/*Transform* transform = GetOwner()->GetComponent<Transform>();

	Vector3 pos = transform->GetPosition();
	Vector3 Rotation = transform->GetRotation();

	std::cout << pos.x << " : X ÁÂÇ¥" << pos.y << " : Y ÁÂÇ¥" << std::endl;

	if (Input::GetInstance()->GetKeyDown(eKeyCode::RBTN))
	{
		Vector2 mousePos = Input::GetInstance()->GetMousePos();
		mArrivePos = Vector3(mousePos.x, mousePos.y, 1.0f);
		mbRun = true;
	}

	if (!mbRun)
		return;

	Vector3 vec = mArrivePos - pos;
	vec.z = 0.0f;

	if (vec.x <= 1.0f && vec.y <= 1.0f)
		mbRun = false;

	vec.Normalize();
	pos += vec * Time::GetInstance()->DeltaTime();

	transform->SetPosition(pos);*/
	Player* player = dynamic_cast<Player*>(GetOwner());

	if (player == nullptr)
		return;

	Transform* tr = player->GetComponent<Transform>();
	Vector3 pos = tr->GetPosition();

	float speed = 3.f;
	/*if (player->GetState() != Player::State::Attack
		&& player->GetState() != Player::State::Skil)
	{
		if (Input::GetInstance()->GetKeyPress(eKeyCode::UP))
		{
			if (player->PlayerDirection(1))
			{
				player->SetState(Player::State::Move);
				pos += speed * tr->Up() * Time::GetInstance()->DeltaTime();
			}
		}
		else if (Input::GetInstance()->GetKeyPress(eKeyCode::DOWN))
		{
			if (player->PlayerDirection(3))
			{
				player->SetState(Player::State::Move);
				pos += speed * -tr->Up() * Time::GetInstance()->DeltaTime();
			}
		}


		if (Input::GetInstance()->GetKeyPress(eKeyCode::RIGHT))
		{
			if (player->PlayerDirection(2))
			{
				player->SetState(Player::State::Move);
				pos += speed * tr->Right() * Time::GetInstance()->DeltaTime();
			}

		}
		else if (Input::GetInstance()->GetKeyPress(eKeyCode::LEFT))
		{
			if (player->PlayerDirection(4))
			{
				player->SetState(Player::State::Move);
				pos += speed * -tr->Right() * Time::GetInstance()->DeltaTime();
			}
		}


		if (player->GetDirection() == 1)
		{
			if (Input::GetInstance()->GetKeyUp(eKeyCode::UP))
				player->SetState(Player::State::Idle);
		}
		else if (player->GetDirection() == 2)
		{
			if (Input::GetInstance()->GetKeyUp(eKeyCode::RIGHT))
				player->SetState(Player::State::Idle);
		}
		else if (player->GetDirection() == 3)
		{
			if (Input::GetInstance()->GetKeyUp(eKeyCode::DOWN))
				player->SetState(Player::State::Idle);
		}
		else if (player->GetDirection() == 4)
		{
			if (Input::GetInstance()->GetKeyUp(eKeyCode::LEFT))
				player->SetState(Player::State::Idle);
		}
	}*/

	if (Input::GetInstance()->GetKeyPress(eKeyCode::A))
	{
		if(player->GetState() == Player::State::Idle
			|| player->GetState() == Player::State::Move)
			player->SetState(Player::State::Attack);
	}
	else if (Input::GetInstance()->GetKeyPress(eKeyCode::S))
	{
		if (player->GetState() == Player::State::Idle
			|| player->GetState() == Player::State::Move)
			player->SetState(Player::State::Skil);
	}


	if (Input::GetInstance()->GetKeyDown(eKeyCode::RBTN))
	{
		Vector2 mouse = Input::GetInstance()->GetMouseWorldPos();
		cout << "X Æ÷Áî  " << mouse.x << "  YÆ÷Áî   " << mouse.y << endl;
		mPickPoint = mouse;
		float angle = PickAngle(mPickPoint);

		Vector2 vec = mPickPoint - Vector2(pos.x, pos.y);

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
		int a = 0;
	}

	if (mPickPoint != Vector2::Zero)
	{
		Vector3 vec = mPickPoint - pos;

		if (fabs(vec.x) < 0.05f && fabs(vec.y) < 0.05f)
		{
			mPickPoint = Vector2::Zero;
			player->SetState(Player::State::Idle);
			return;
		}

		vec.Normalize();
		pos += vec * Time::GetInstance()->DeltaTime() * speed;
	}

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

float PlayerScript::PickAngle(Vector2 point)
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
