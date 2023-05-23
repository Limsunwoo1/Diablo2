#include "CCameraScript.h"
#include "CGameObject.h"
#include "CInput.h"
#include "CTime.h"
#include "CSceneManager.h"
#include "CCamera.h"
#include "CWorldManager.h"

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
	//if (SceneManager::GetInstance()->GetActiveScene()->GetScenType() != eSceneType::Tool)
	//	return;

	if (Input::GetInstance()->GetKeyDown(eKeyCode::SPACE))
	{
		Camera* camera = GetOwner()->GetComponent<Camera>();
		if (camera != nullptr)
		{
			GameObject* player = WorldManager::GetInstance()->GetPlayer();
			camera->GetTrace() == nullptr ? camera->SetTrace(player) : camera->SetTrace(nullptr);
		}
	}

	Transform* tr = GetOwner()->GetComponent<Transform>();

	Vector3 pos = tr->GetPosition();
	
	float speed = 500.f;

	if (Input::GetInstance()->GetkeyState(eKeyCode::D) == eKeyState::PRESSED)
	{
		pos += speed * tr->Right() * Time::GetInstance()->DeltaTime();
	}
	else if (Input::GetInstance()->GetkeyState(eKeyCode::A) == eKeyState::PRESSED)
	{
		pos += speed * -tr->Right() * Time::GetInstance()->DeltaTime();
	}
	else if (Input::GetInstance()->GetkeyState(eKeyCode::Q) == eKeyState::PRESSED)
	{
		pos += speed * tr->Forward() * Time::GetInstance()->DeltaTime();
	}
	else if (Input::GetInstance()->GetkeyState(eKeyCode::E) == eKeyState::PRESSED)
	{
		pos += speed * -tr->Forward() * Time::GetInstance()->DeltaTime();
	}
	else if (Input::GetInstance()->GetkeyState(eKeyCode::W) == eKeyState::PRESSED)
	{
		pos += speed * tr->Up() * Time::GetInstance()->DeltaTime();
	}
	else if (Input::GetInstance()->GetkeyState(eKeyCode::S) == eKeyState::PRESSED)
	{
		pos += speed * -tr->Up() * Time::GetInstance()->DeltaTime();
	}
	else if (Input::GetInstance()->GetkeyState(eKeyCode::H) == eKeyState::PRESSED)
	{
		Vector3 ro = tr->GetRotation();
		ro.x += 10.f * Time::GetInstance()->DeltaTime();
		tr->SetRotation(ro);
	}
	else if (Input::GetInstance()->GetkeyState(eKeyCode::J) == eKeyState::PRESSED)
	{
		Vector3 ro = tr->GetRotation();
		ro.y += 10.f * Time::GetInstance()->DeltaTime();
		tr->SetRotation(ro);
	}
	else if (Input::GetInstance()->GetkeyState(eKeyCode::K) == eKeyState::PRESSED)
	{
		Vector3 ro = tr->GetRotation();
		ro.z += 10.f * Time::GetInstance()->DeltaTime();
		tr->SetRotation(ro);
	}
	else if (Input::GetInstance()->GetkeyState(eKeyCode::B) == eKeyState::PRESSED)
	{
		Vector3 ro = tr->GetRotation();
		ro.x -= 10.f * Time::GetInstance()->DeltaTime();
		tr->SetRotation(ro);
	}
	else if (Input::GetInstance()->GetkeyState(eKeyCode::N) == eKeyState::PRESSED)
	{
		Vector3 ro = tr->GetRotation();
		ro.y -= 10.f * Time::GetInstance()->DeltaTime();
		tr->SetRotation(ro);
	}
	else if (Input::GetInstance()->GetkeyState(eKeyCode::M) == eKeyState::PRESSED)
	{
		Vector3 ro = tr->GetRotation();
		ro.z -= 10.f * Time::GetInstance()->DeltaTime();
		tr->SetRotation(ro);
	}

	tr->SetPosition(pos);
}

void CameraScript::FixedUpdate()
{
}

void CameraScript::Render()
{
}
