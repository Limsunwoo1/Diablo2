#pragma once
#include "CCollider2D.h"
class MonsterSkilCollider : public Collider2D
{
public:
	MonsterSkilCollider();
	virtual ~MonsterSkilCollider();

	virtual void Initalize() override;
	virtual void Update() override;
	virtual void FixedUpdate() override;
	virtual void Render() override;

	virtual void OnCollisionEnter(Collider2D* collider);

private:


};

