#include "CPlayerScript.h"
#include "CTransform.h"
#include "CGameObject.h"
#include "CInput.h"
#include "CTime.h"

PlayerScript::PlayerScript()
	: Script()
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

	if (Input::GetInstance()->GetkeyState(eKeyCode::R) == eKeyState::PRESSED)
	{
		Rotation.z += 10.0 * Time::GetInstance()->DeltaTime();
		transform->SetRotation(Rotation);
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
	}

	transform->SetPosition(pos);


	
	
}

void PlayerScript::FixedUpdate()
{
}

void PlayerScript::Render()
{
}
