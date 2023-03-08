#pragma once
#include "CGameObject.h"
class Player : public GameObject
{
public:
	Player();
	virtual ~Player();

	virtual void Initalize();
	virtual void Update();
	virtual void FixedUpdate();
	virtual void Render();

private:
	UINT mHp;
	UINT mMP;
	UINT mSpeed;

	std::bitset<16> mDirection;
	
};

