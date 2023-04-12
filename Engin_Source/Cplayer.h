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

	void SetRunMode(bool mode) { mbRunMode = mode; }
	bool GetRunMode() { return mbRunMode; }

	void ChangeRunMode();

	void SetRunTime(float time) { mRunTime = time; }
	float GetRunTime() { return mRunTime; }

	void SetRunSpeed(bool mode);
	UINT GetRunSpeed() { return mRunSpeed; };

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

	// 0 (6시 방향) 4(9시 방향) 8(12시방향) 12(3시 방향) 
	std::bitset<16> mDirection;
	UINT mIndex;
	State mState;
	bool mbRunMode;
	float mRunTime;
	float mMaxRunTime;

	// 걷기 2, 뛰기 4
	UINT mRunSpeed;
};

