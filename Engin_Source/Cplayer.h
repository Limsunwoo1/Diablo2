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

	void ActiveSkilFireBall();

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

	// 0 (6�� ����) 4(9�� ����) 8(12�ù���) 12(3�� ����) 
	std::bitset<16> mDirection;
	UINT mIndex;
	State mState;
};

