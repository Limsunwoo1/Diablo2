#pragma once
#include "CMonster.h"

class MinosTauros : public Monster
{
public:
	MinosTauros();
	virtual ~MinosTauros();

	virtual void Initalize() override;
	virtual void Update() override;
	virtual void FixedUpdate() override;
	virtual void Render() override;

	virtual void InitAnimation() override;

	virtual void Idle() override;
	virtual void Move() override;
	virtual void Attack() override;
	virtual void Hit() override;
	virtual void Dead() override;
	virtual void HitFire() override;
	virtual void HitFrozen() override;
	virtual void HitLight() override;
private:
	
};

