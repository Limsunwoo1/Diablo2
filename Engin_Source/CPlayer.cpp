#include "CPlayer.h"

Player::Player()
	: GameObject()
	, mHp(0)
	, mMP(0)
	, mSpeed(10)
{
	// 0(12�ù���)
	mDirection[0] = 1;
}

Player::~Player()
{
}

void Player::Initalize()
{
	GameObject::Initalize();
}

void Player::Update()
{
	GameObject::Update();
}

void Player::FixedUpdate()
{
	GameObject::FixedUpdate();
}

void Player::Render()
{
	GameObject::Render();
}
