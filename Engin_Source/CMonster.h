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

	void SetMonsterStatusEffect(eElementType state) { mMonsterStatusEffect = state; mElementTime = 3.0f; }
	eElementType GetMonsterStatusEffect() { return mMonsterStatusEffect; }

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

	float GetDamege() { return mDamege; }
	void SetDamege(float damege) { mDamege = damege; }

	void CheckPointCollision();

	virtual void Attack();
	virtual void GetHit(){};
	virtual void DropItem();


protected:
	virtual void idle()			{};
	virtual void move()			{};
	virtual void attack()		{};
	virtual void hit()			{};
	virtual void monsterDead()	{};
	virtual void hitFire();
	virtual void hitFrozen();
	virtual void hitLight();
	virtual void discountElement();
private:
	Vector2 mSpawnPos;
	// 0(↑) 2(→) 4(↓) 6(←)
	std::bitset<8> mDirection;
	UINT mIndex;
	MonsterState mMonsterState;
	eElementType mMonsterStatusEffect;
	GameObject* mTarget;

	float mMaxHP;
	float mHP;

	float mDeltaTime;
	float mDotDamageCoolTime;
	float mElementTime;

	bool mReset;

	float mDamege;
	float mLightElementDamege;


protected:
	GameObject* mShadow;
	Vector2 AttackSize;
};
