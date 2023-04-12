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
		Dead
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
private:
	MonsterState mMonsterState;
};

