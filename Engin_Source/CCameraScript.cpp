#include "CCameraScript.h"
#include "CTransform.h"
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
		pos.x += 3.0f * Time::GetInstance()->DeltaTime();
	}
	else if (Input::GetInstance()->GetkeyState(eKeyCode::A) == eKeyState::PRESSED)
	{
		pos.x -= 3.0f * Time::GetInstance()->DeltaTime();
	}
	else if (Input::GetInstance()->GetkeyState(eKeyCode::W) == eKeyState::PRESSED)
	{
		pos.z += 3.0f * Time::GetInstance()->DeltaTime();
	}
	else if (Input::GetInstance()->GetkeyState(eKeyCode::S) == eKeyState::PRESSED)
	{
		pos.z -= 3.0f * Time::GetInstance()->DeltaTime();
	}
	else if (Input::GetInstance()->GetkeyState(eKeyCode::Q) == eKeyState::PRESSED)
	{
		pos.y -= 3.0f * Time::GetInstance()->DeltaTime();
	}
	else if (Input::GetInstance()->GetkeyState(eKeyCode::E) == eKeyState::PRESSED)
	{
		pos.y += 3.0f * Time::GetInstance()->DeltaTime();
	}

	tr->SetPosition(pos);
}

void CameraScript::FixedUpdate()
{
}

void CameraScript::Render()
{
}
