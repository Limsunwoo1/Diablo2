#include "CMonsterSkilCollider.h"
#include "CMonster.h"
#include "Cplayer.h"
#include "CSkil.h"

MonsterSkilCollider::MonsterSkilCollider()
	: Collider2D()
{
}

MonsterSkilCollider::~MonsterSkilCollider()
{
}

void MonsterSkilCollider::Initalize()
{
	Collider2D::Initalize();

}

void MonsterSkilCollider::Update()
{
	Collider2D::Update();
}

void MonsterSkilCollider::FixedUpdate()
{
	Collider2D::FixedUpdate();
}

void MonsterSkilCollider::Render()
{
	Collider2D::Render();
}

void MonsterSkilCollider::OnCollisionEnter(Collider2D* collider)
{
	GameObject* obj = collider->GetOwner();
	Player* player = dynamic_cast<Player*>(obj);
	if (player == nullptr)
		return;

	Skil* skil = dynamic_cast<Skil*>(GetOwner());
	if (skil == nullptr)
		return;

	float hp = player->GetHP();
	float damage = skil->GetDamege();

	hp -= damage;
	player->SetHP(hp);

	skil->Paused();
}
