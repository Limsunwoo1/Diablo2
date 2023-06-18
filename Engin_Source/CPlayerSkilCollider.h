#pragma once
#include "CCollider2D.h"
class PlayerSkilCollider : public Collider2D
{
public:
	PlayerSkilCollider();
	virtual ~PlayerSkilCollider();

	virtual void Initalize() override;
	virtual void Update() override;
	virtual void FixedUpdate() override;
	virtual void Render() override;

	virtual void OnCollisionEnter(Collider2D* collider);

private:


};

