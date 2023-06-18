#include "CAttackCollider.h"
#include "CMonster.h"
#include "Cplayer.h"
#include "CSkil.h"

AttackCollider::AttackCollider()
	: Collider2D()
{
}

AttackCollider::~AttackCollider()
{
}

void AttackCollider::Initalize()
{
	Collider2D::Initalize();

}

void AttackCollider::Update()
{
	Collider2D::Update();
}

void AttackCollider::FixedUpdate()
{
	Collider2D::FixedUpdate();
}

void AttackCollider::Render()
{
	Collider2D::Render();
}

void AttackCollider::OnCollisionEnter(Collider2D* collider)
{
	GameObject* mOwner = GetOwner();
	GameObject* other = collider->GetOwner();

	eLayerType ownerType = mOwner->GetLayerType();

	float OwnerDamege = 0.0f;
	float otherhp = 0.0f;

	if (ownerType == eLayerType::Player)
	{
		Player* player = CastType<Player>(ownerType,mOwner);
		OwnerDamege = player->GetDamege();

		Monster* monster = CastType<Monster>(other->GetLayerType(), other);
		otherhp = monster->GetHP();

		otherhp -= OwnerDamege;
		monster->SetHP(otherhp);

		return;
	}

	Monster* monster = CastType<Monster>(ownerType, mOwner);
	OwnerDamege = monster->GetDamege();

	Player* player = CastType<Player>(other->GetLayerType(), other);
	otherhp = player->GetHP();

	otherhp -= OwnerDamege;
	player->SetHP(otherhp);
}
