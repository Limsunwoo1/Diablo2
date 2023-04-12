#pragma once
#include "CGameObject.h"


class Player : public GameObject
{
public:
	enum class PlayerState
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

	PlayerState GetState() { return mState; }
	void SetState(PlayerState state);

	bool PlayerDirection(int index);
	UINT GetDirection();

	void SetRunMode(bool mode) { mbRunMode = mode; }
	bool GetRunMode() { return mbRunMode; }

	void ChangeRunMode();

	void SetRunTime(float time) { mRunTime = time; }
	float GetRunTime() { return mRunTime; }

	void SetRunSpeed(bool mode);
	UINT GetRunSpeed() { return mRunSpeed; };

	const float GetMaxRunTime() { return mMaxRunTime; }
	void ActiveSkilFireBall();

private:
	void Run();
	void Idle();
	void Move();
	void Attack();
	void SKil();
	void Hit();
private:
	float mMaxHP;
	float mHP;

	float mMaxMP;
	float mMP;

	float mDamege;

	// 0 (6시 방향) 4(9시 방향) 8(12시방향) 12(3시 방향) 
	std::bitset<16> mDirection;
	UINT mIndex;
	PlayerState mState;
	bool mbRunMode;

	float mRunTime;
	float mMaxRunTime;

	// 걷기 2, 뛰기 4
	UINT mRunSpeed;
};

