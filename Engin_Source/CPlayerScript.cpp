#include "CPlayerScript.h"
#include "CTransform.h"
#include "CGameObject.h"
#include "CInput.h"
#include "CTime.h"

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
}

void PlayerScript::Update()
{
	Transform* transform = GetOwner()->GetComponent<Transform>();

	Vector3 pos = transform->GetPosition();
	Vector3 Rotation = transform->GetRotation();

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

	transform->SetPosition(pos);
	/*if (Input::GetInstance()->GetkeyState(eKeyCode::R) == eKeyState::PRESSED)
	{
		Rotation.z += 10.0 * Time::GetInstance()->DeltaTime();
		transform->SetRotation(Rotation);
	}

	if (Input::GetInstance()->GetkeyState(eKeyCode::LEFT) == eKeyState::PRESSED)
	{
		Vector3 pos = transform->GetPosition();
		pos.x -= 6.0f * Time::GetInstance()->DeltaTime();
		transform->SetPosition(pos);
	}

	else if (Input::GetInstance()->GetkeyState(eKeyCode::RIGHT) == eKeyState::PRESSED)
	{
		Vector3 pos = transform->GetPosition();
		pos.x += 6.0f * Time::GetInstance()->DeltaTime();
		transform->SetPosition(pos);
	}

	if (Input::GetInstance()->GetkeyState(eKeyCode::DOWN) == eKeyState::PRESSED)
	{
		Vector3 pos = transform->GetPosition();
		pos.z -= 6.0f * Time::GetInstance()->DeltaTime();
		transform->SetPosition(pos);
	}
	else if (Input::GetInstance()->GetkeyState(eKeyCode::UP) == eKeyState::PRESSED)
	{
		Vector3 pos = transform->GetPosition();
		pos.z += 6.0f * Time::GetInstance()->DeltaTime();
		transform->SetPosition(pos);
	}

	if (Input::GetInstance()->GetkeyState(eKeyCode::W) == eKeyState::PRESSED)
	{
		pos.y += 1.0f * Time::GetInstance()->DeltaTime();
	}
	else if (Input::GetInstance()->GetkeyState(eKeyCode::S) == eKeyState::PRESSED)
	{
		pos.y -= 1.0f * Time::GetInstance()->DeltaTime();
	}

	if (Input::GetInstance()->GetkeyState(eKeyCode::D) == eKeyState::PRESSED)
	{
		pos.x += 1.0f * Time::GetInstance()->DeltaTime();
	}
	else if (Input::GetInstance()->GetkeyState(eKeyCode::A) == eKeyState::PRESSED)
	{
		pos.x -= 1.0f * Time::GetInstance()->DeltaTime();
	}*/

	//transform->SetPosition(pos);
}

void PlayerScript::FixedUpdate()
{
}

void PlayerScript::Render()
{
}
