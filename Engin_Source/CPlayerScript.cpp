#include "CPlayerScript.h"
#include "CTransform.h"
#include "CGameObject.h"
#include "CInput.h"
#include "CTime.h"
#include "CAnimator.h"

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
	Transform* transform = GetOwner()->GetComponent<Transform>();

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

	transform->SetPosition(pos);

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
