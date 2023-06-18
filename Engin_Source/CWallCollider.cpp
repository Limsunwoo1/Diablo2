#include "CWallCollider.h"
#include "CMonster.h"
#include "CSkil.h"

WallCollider::WallCollider()
	: Collider2D()
{
}

WallCollider::~WallCollider()
{
}

void WallCollider::Initalize()
{
	Collider2D::Initalize();

}

void WallCollider::Update()
{
	Collider2D::Update();
}

void WallCollider::FixedUpdate()
{
	Collider2D::FixedUpdate();
}

void WallCollider::Render()
{
	Collider2D::Render();
}

void WallCollider::OnCollisionEnter(Collider2D* collider)
{
	GameObject* obj = collider->GetOwner();
	Skil* skil = dynamic_cast<Skil*>(obj);
	if (skil == nullptr)
		return;

	skil->Paused();
}
