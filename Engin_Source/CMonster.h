#pragma once
#include "CGameObject.h"

class Monster : public GameObject
{
public:
	enum class MonsterState
	{
		Idle,
		Move,
		Attack,
		Hit,
		Dead,
		HitFire,
		HitFrozen,
		HitLight,
		End,
	};

public:
	Monster();
	virtual ~Monster();

	virtual void Initalize() override;
	virtual void Update() override;
	virtual void FixedUpdate() override;
	virtual void Render() override;

	virtual void InitAnimation() override {};

	void SetMonsterState(MonsterState state) { mMonsterState = state; }
	MonsterState GetMonsterState() { return mMonsterState; }

	float GetMaxHp() { return mMaxHP; }
	void SetMaxHp(float hp) { mMaxHP = hp; }

	float GetHP() { return mHP; }
	void SetHP(float hp) { mHP = hp; }

	float GetCurDeltaTime() { return mDeltaTime; }
	void SetCurDeltaTime(float delta) { mDeltaTime = delta; }

	void Run();

	virtual void Idle()			{};
	virtual void Move()			{};
	virtual void Attack()		{};
	virtual void Hit()			{};
	virtual void Dead()			{};
	virtual void HitFire()		{};
	virtual void HitFrozen()	{};
	virtual void HitLight()		{};
private:
	MonsterState mMonsterState;

	float mMaxHP;
	float mHP;

	float mDeltaTime;
};

