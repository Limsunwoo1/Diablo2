#pragma once
#include "CMonster.h"

class DiabloMonster : public Monster
{
public:
	DiabloMonster();
	virtual ~DiabloMonster();

	virtual void Initalize() override;
	virtual void Update() override;
	virtual void FixedUpdate() override;
	virtual void Render() override;

	virtual void InitAnimation() override;

	float GetSkilCurTime() { return mSkilCurTime; }
	float GetSkilCoolTime() { return mSkilCoolTime; }

	void CreateNextScenePotal();
	void DiabloSpecialCast1();
	void DiabloSpecialCast2();


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
	std::shared_ptr<class Material> mMaterial;

	float mSkilCoolTime;
	float mSkilCurTime;
};

