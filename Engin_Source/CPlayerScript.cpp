#include "CPlayerScript.h"
#include "CTransform.h"
#include "CGameObject.h"
#include "CInput.h"
#include "CTime.h"
#include "CAnimator.h"
#include "CPlayer.h"

PlayerScript::PlayerScript()
	: Script()
	, mArrivePos(Vector3::Zero)
	, mbRun(false)
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
		if(Input::GetInstance()->GetKeyUp(eKeyCode::UP))
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

	if (Input::GetInstance()->GetKeyPress(eKeyCode::A))
	{
		player->SetState(Player::State::Attack);
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
