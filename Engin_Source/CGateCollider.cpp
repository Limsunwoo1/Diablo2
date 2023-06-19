#include "CGateCollider.h"
#include "CGateObject.h"
#include "CInput.h"
#include "CWorldManager.h"
#include "CCollisionManager.h"

GateCollider::GateCollider()
	: Collider2D()
{
}

GateCollider::~GateCollider()
{
}

void GateCollider::Initalize()
{
	Collider2D::Initalize();
}

void GateCollider::Update()
{
	Collider2D::Update();
}

void GateCollider::FixedUpdate()
{
	Collider2D::FixedUpdate();
}

void GateCollider::Render()
{
	Collider2D::Render();
}

void GateCollider::OnCollisionEnter(Collider2D* collider)
{

}

void GateCollider::OnCollisionStay(Collider2D* collider)
{
	Vector2 MousePos = Input::GetInstance()->GetMouseWorldPos(true);
	bool MousePick = false;

	MousePick = CollisionManager::GetInstance()->AABBRect_VS_Point(this, MousePos);

	if (MousePick == false)
		return;

	if (Input::GetInstance()->GetKeyPress(eKeyCode::RBTN))
	{
		GateObject* obj = dynamic_cast<GateObject*>(GetOwner());
		if (obj == nullptr)
			return;

		obj->NextScene();
	}
}
