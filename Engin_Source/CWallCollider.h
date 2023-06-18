#pragma once
#include "CCollider2D.h"
class WallCollider : public Collider2D
{
public:
	WallCollider();
	virtual ~WallCollider();

	virtual void Initalize() override;
	virtual void Update() override;
	virtual void FixedUpdate() override;
	virtual void Render() override;

	virtual void OnCollisionEnter(Collider2D* collider);

private:


};

