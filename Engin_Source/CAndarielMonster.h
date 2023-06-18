#pragma once
#include "CMonster.h"

class AndarielMonster : public Monster
{
public:
	AndarielMonster();
	virtual ~AndarielMonster();

	virtual void Initalize() override;
	virtual void Update() override;
	virtual void FixedUpdate() override;
	virtual void Render() override;

	virtual void InitAnimation() override;

	float GetSkilCurTime() { return mSkilCurTime; }
	float GetSkilCoolTime() { return mSkilCoolTime; }

	void CreateNextScenePotal();
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
	std::shared_ptr<class Material> mOverlayMaterial;
	GameObject* mOverlay;

	float mSkilCoolTime;
	float mSkilCurTime;
};



