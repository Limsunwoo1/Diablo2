#pragma once
#include "CGameObject.h"


class Player : public GameObject
{
public:
	enum class State
	{
		Idle,
		Move,
		Attack,
		Skil,
		Hit,
		End,
	};

public:
	Player();
	virtual ~Player();

	virtual void Initalize();
	virtual void Update();
	virtual void FixedUpdate();
	virtual void Render();

	virtual void InitAnimation() override;
	void PlayAnimation(const std::wstring& name);

	State GetState() { return mState; }
	void SetState(State state);

	bool PlayerDirection(int index);
	UINT GetDirection();

private:
	void Run();
	void Idle();
	void Move();
	void Attack();
	void SKil();
	void Hit();
private:
	UINT mHP;
	UINT mMP;

	UINT mDamege;

	// (1)12��, 2(3��), 3(6��), 4(9��) 
	std::bitset<4> mDirection;
	State mState;
};

