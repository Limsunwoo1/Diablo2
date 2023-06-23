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

	virtual void Attack() override;
	virtual void GetHit() override;

protected:
	virtual void idle() override;
	virtual void move() override;
	virtual void attack() override;
	virtual void hit() override;
	virtual void monsterDead() override;
	virtual void hitFire() override;
	virtual void hitFrozen() override;
	virtual void hitLight() override;
private:
	
};

