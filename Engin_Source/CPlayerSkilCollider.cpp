#include "CPlayerSkilCollider.h"
#include "CMonster.h"
#include "CSkil.h"

PlayerSkilCollider::PlayerSkilCollider()
	: Collider2D()
{
}

PlayerSkilCollider::~PlayerSkilCollider()
{
}

void PlayerSkilCollider::Initalize()
{
	Collider2D::Initalize();

}

void PlayerSkilCollider::Update()
{
	Collider2D::Update();
}

void PlayerSkilCollider::FixedUpdate()
{
	Collider2D::FixedUpdate();
}

void PlayerSkilCollider::Render()
{
	Collider2D::Render();
}

void PlayerSkilCollider::OnCollisionEnter(Collider2D* collider)
{
	GameObject* obj = collider->GetOwner();
	Monster* monster = dynamic_cast<Monster*>(obj);
	if (monster == nullptr)
		return;

	Skil* skil = dynamic_cast<Skil*>(GetOwner());
	if (skil == nullptr)
		return;

	float hp = monster->GetHP();
	float damage = skil->GetDamege();

	hp -= damage;
	monster->SetHP(hp);

	monster->SetMonsterStatusEffect(skil->GetElementType());

	skil->Paused();
}
