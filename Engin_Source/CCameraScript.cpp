#include "CCameraScript.h"
#include "CGameObject.h"
#include "CInput.h"
#include "CTime.h"

CameraScript::CameraScript()
	: Script()
{
}

CameraScript::~CameraScript()
{
}

void CameraScript::Initalize()
{
}

void CameraScript::Update()
{
	Transform* tr = GetOwner()->GetComponent<Transform>();

	Vector3 pos = tr->GetPosition();

	if (Input::GetInstance()->GetkeyState(eKeyCode::D) == eKeyState::PRESSED)
	{
		pos += 100.f * tr->Right() * Time::GetInstance()->DeltaTime();
	}
	else if (Input::GetInstance()->GetkeyState(eKeyCode::A) == eKeyState::PRESSED)
	{
		pos += 100.f * -tr->Right() * Time::GetInstance()->DeltaTime();
	}
	else if (Input::GetInstance()->GetkeyState(eKeyCode::W) == eKeyState::PRESSED)
	{
		pos += 100.f * tr->Forward() * Time::GetInstance()->DeltaTime();
	}
	else if (Input::GetInstance()->GetkeyState(eKeyCode::S) == eKeyState::PRESSED)
	{
		pos += 100.f * -tr->Forward() * Time::GetInstance()->DeltaTime();
	}
	else if (Input::GetInstance()->GetkeyState(eKeyCode::Q) == eKeyState::PRESSED)
	{
		pos += 100.f * tr->Up() * Time::GetInstance()->DeltaTime();
	}
	else if (Input::GetInstance()->GetkeyState(eKeyCode::E) == eKeyState::PRESSED)
	{
		pos += 100.f * -tr->Up() * Time::GetInstance()->DeltaTime();
	}

	tr->SetPosition(pos);
}

void CameraScript::FixedUpdate()
{
}

void CameraScript::Render()
{
}
