#pragma once
#include "CGameObject.h"
#include "CAStar.h"

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
		End,
	};

	enum class MonsterStatusEffect
	{
		None,
		HitFire,
		HitFrozen,
		HitLight,
	};

public:
	Monster();
	virtual ~Monster();

	virtual void Initalize() override;
	virtual void Update() override;
	virtual void FixedUpdate() override;
	virtual void Render() override;

	virtual void InitAnimation() override {};

	void StatusEffect();

	void SetMonsterState(MonsterState state) { mMonsterState = state; }
	MonsterState GetMonsterState() { return mMonsterState; }

	void SetMonsterStatusEffect(MonsterStatusEffect state) { mMonsterStatusEffect = state; }
	MonsterStatusEffect GetMonsterStatusEffect() { return mMonsterStatusEffect; }

	float GetMaxHp() { return mMaxHP; }
	void SetMaxHp(float hp) { mMaxHP = hp; }

	float GetHP() { return mHP; }
	void SetHP(float hp) { mHP = hp; }

	float GetCurDeltaTime() { return mDeltaTime; }
	void SetCurDeltaTime(float delta) { mDeltaTime = delta; }

	bool MonsterDirection(int index);
	UINT GetDirection();

	UINT GetIndex() { return mIndex; }
	void SetIndex(UINT index) { mIndex = index; }

	// 탐색범위 혹은 공격 받으면 위치를 추적할 플레이어를 
	// 포인터로 갖고 있는다
	GameObject* GetTarget() { return mTarget; }
	void SetTarget(GameObject* player) { mTarget = player; }

	Vector2 GetSpawnPos() { return mSpawnPos; }
	void SetSpawnPos(Vector2& pos) { mSpawnPos = pos; }

	void Run();

protected:
	virtual void idle()			{};
	virtual void move()			{};
	virtual void attack()		{};
	virtual void hit()			{};
	virtual void monsterDead()	{};
	virtual void hitFire()		{};
	virtual void hitFrozen()	{};
	virtual void hitLight()		{};
private:
	Vector2 mSpawnPos;
	// 0(↑) 2(→) 4(↓) 6(←)
	std::bitset<8> mDirection;
	UINT mIndex;
	MonsterState mMonsterState;
	MonsterStatusEffect mMonsterStatusEffect;
	GameObject* mTarget;

	float mMaxHP;
	float mHP;

	float mDeltaTime;
	float mDotDamageCoolTime;

	bool mReset;
};
