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

	void SetCharType(eCharType type) { mCharType = (UINT)type; }
	eCharType GetCharType() { return (eCharType)mCharType; }

	void SetHP(float hp) { mHP = hp; }
	float GetHP() { return mHP; }

	void SetMP(float mp) { mMP = mp; }
	float GetMP() { return mMP; }

	void SetExp(float exp) { mExp = exp; }
	float GetExp() { return mExp; }

	void SetMaxExp(float exp) { mMaxExp = exp; }
	float GetMaxExp() { return mMaxExp; }

	void SetLevel(int level) { mLevel = level; }
	int GetLevel() { return mLevel; }

	const float GetMaxRunTime() { return mMaxRunTime; }
	void ActiveSkilFireBall();

	const string& GetSavename() { return mSaveName; }
	void SetSaveName(const string& name) { mSaveName = name; }

	const UINT GetSavePathIndex() { return mSavePathIndex; }
	void SetSavePathIndex(UINT index) { mSavePathIndex = index; }

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
	UINT mCharType;
	float mExp;
	float mMaxExp;
	
	int mLevel;

	// 0 (6시 방향) 4(9시 방향) 8(12시방향) 12(3시 방향) 
	std::bitset<16> mDirection;
	UINT mIndex;
	PlayerState mState;
	bool mbRunMode;

	float mRunTime;
	float mMaxRunTime;

	// 걷기 2, 뛰기 3
	UINT mRunSpeed;

	string mSaveName;
	UINT mSavePathIndex;

	GameObject* mShadow;
};

