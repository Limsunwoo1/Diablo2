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
		Player* player = nullptr;
		player = CastType<Player>(ownerType,mOwner);
		if(player != nullptr)
			OwnerDamege = player->GetDamege();


		Monster* monster = nullptr;
		monster = CastType<Monster>(other->GetLayerType(), other);
		if(monster != nullptr)
			otherhp = monster->GetHP();

		otherhp -= OwnerDamege;
		monster->SetHP(otherhp);

		return;
	}

	Monster* monster = nullptr;
	monster = CastType<Monster>(ownerType, mOwner);
	if(monster != nullptr)
		OwnerDamege = monster->GetDamege();

	Player* player = nullptr;
	player = CastType<Player>(other->GetLayerType(), other);
	if (player != nullptr)
		otherhp = player->GetHP();

	otherhp -= OwnerDamege;
	player->SetHP(otherhp);
}
