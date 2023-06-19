#pragma once
#include "CCollider2D.h"

class GateCollider : public Collider2D
{
public:
	GateCollider();
	virtual ~GateCollider();

	virtual void Initalize() override;
	virtual void Update() override;
	virtual void FixedUpdate() override;
	virtual void Render() override;

	virtual void OnCollisionEnter(Collider2D* collider);
	virtual void OnCollisionStay(Collider2D* collider);
private:

};

