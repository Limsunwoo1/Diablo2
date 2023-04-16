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

	void StatusEffect();

	void SetMonsterState(MonsterState state) { mMonsterState = state; }
	MonsterState GetMonsterState() { return mMonsterState; }

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

	// Ž������ Ȥ�� ���� ������ ��ġ�� ������ �÷��̾ 
	// �����ͷ� ���� �ִ´�
	GameObject* GetTarget() { return mTarget; }
	void SetTarget(GameObject* player) { mTarget = player; }

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
	// 0(��) 2(��) 4(��) 6(��)
	std::bitset<8> mDirection;
	UINT mIndex;
	MonsterState mMonsterState;
	GameObject* mTarget;

	float mMaxHP;
	float mHP;

	float mDeltaTime;
	float mDotDamageCoolTime;
};
