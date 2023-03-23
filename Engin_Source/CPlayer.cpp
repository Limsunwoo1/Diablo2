#include "CPlayer.h"
#include "CResourceManager.h"
#include "CSpriteRenderer.h"
#include "CMaterial.h"
#include "CTexture2D.h"
#include "CAnimator.h"

Player::Player()
	: GameObject()
	, mHP(10)
	, mMP(10)
	, mDamege(5)
	, mIndex(0)
{

}

Player::~Player()
{

}

void Player::Initalize()
{
	mDirection.reset();
	mDirection[2] = 1;

	GameObject::Initalize();
}

void Player::Update()
{
	GameObject::Update();
	Run();
}

void Player::FixedUpdate()
{
	GameObject::FixedUpdate();
}

void Player::Render()
{
	SpriteRenderer* spr = GetComponent<SpriteRenderer>();
	std::shared_ptr<Texture2D> texture = ResourceManager::GetInstance()->Find<Texture2D>(L"test");
	spr->GetMaterial()->SetTexture(texture);


	GameObject::Render();
}

void Player::InitAnimation()
{
	Animator* animator = this->AddComponent<Animator>();
	{
		std::shared_ptr<Texture2D> texture = std::make_shared<Texture2D>();
		texture->Load(L"Diablo2_Walk.png");
		ResourceManager::GetInstance()->Insert(L"PlayerWalk", texture);

		/*animator->Create(L"Walk0", texture, Vector2(0.0f, 0.0f), Vector2(60.f, 75.875f), Vector2(0.0f, 0.0f), 8, 0.1f);
		animator->Create(L"WalkUp", texture, Vector2(0.0f, 75.875 * 8), Vector2(60.f, 75.875f), Vector2(0.0f, 0.0f), 8, 0.1f);*/

		float x = 60.f;
		float y = 75.875f;

		for (int i = 0; i < mDirection.size(); ++i)
		{
			wstring name = L"Walk";
			name += std::to_wstring(i);

			animator->Create(name, texture, Vector2(0.0f, y * i), Vector2(x, y), Vector2(0.0f, 0.0f), 8, 0.1f);
		}
		//60 75.875
	}

	{
		std::shared_ptr<Texture2D> texture = std::make_shared<Texture2D>();
		texture->Load(L"Diablo2_Idle.png");
		ResourceManager::GetInstance()->Insert(L"PlayerIdle", texture);

		float x = 66.f;
		float y = 72.f;

		for (int i = 0; i < mDirection.size(); ++i)
		{
			wstring name = L"Idle";
			name += std::to_wstring(i);

			animator->Create(name, texture, Vector2(0.0f, y * i), Vector2(x, y), Vector2(0.0f, 0.0f), 8, 0.1f);
		}

		// 66  72
	}

	{
		std::shared_ptr<Texture2D> texture = std::make_shared<Texture2D>();
		texture->Load(L"attack1.png");
		ResourceManager::GetInstance()->Insert(L"PlayerAttack", texture);

		float x = 124.95f;
		float y = 78.8125f;

		for (int i = 0; i < mDirection.size(); ++i)
		{
			wstring name = L"Attack";
			name += std::to_wstring(i);

			animator->Create(name, texture, Vector2(0.0f, y * i), Vector2(x, y), Vector2(0.0f, 0.0f), 8, 0.1f);

		}
		// 124.95  78.8125
	}

	{
		std::shared_ptr<Texture2D> texture = std::make_shared<Texture2D>();
		texture->Load(L"skill1.png");
		ResourceManager::GetInstance()->Insert(L"PlayerSKil", texture);

		float x = 88.7857142857f;
		float y = 91.9375f;

		for (int i = 0; i < mDirection.size(); ++i)
		{
			wstring name = L"Skil";
			name += std::to_wstring(i);

			animator->Create(name, texture, Vector2(0.0f, y * i), Vector2(x, y), Vector2(0.0f, 0.0f), 8, 0.1f);

		}

		// 88.7857142857 91.9375
	}
}

void Player::PlayAnimation(const std::wstring& name)
{
	Animator* animator = this->GetComponent<Animator>();
	if (animator)
		animator->Play(name);
}

bool Player::PlayerDirection(int index)
{
	if (index - 1 > mDirection.size())
		return false;

	mDirection.reset();
	mDirection[index] = 1;
	mIndex = index;

	return true;
}

UINT Player::GetDirection()
{
	// index Å½»ö
	/*for (int i = 0; i < mDirection.size(); ++i)
	{
		if (mDirection[i] > 0)
			return i;
	}*/
	return mIndex;
}

void Player::SetState(State state)
{
	if (state == mState)
		return;

	mState = state;
}

void Player::Run()
{
	switch (mState)
	{
	case Player::State::Idle:
		Idle();
		break;
	case Player::State::Move:
		Move();
		break;
	case Player::State::Attack:
		Attack();
		break;
	case Player::State::Skil:
		SKil();
		break;
	case Player::State::Hit:
		Hit();
		break;
	default:
		break;
	}
}

void Player::Idle()
{
	Animator* animator = GetComponent<Animator>();
	std::wstring& name = animator->GetPlayAnimation()->AnimationName();

	int index = -1;


	if (mDirection[0] == true && name == L"WalkUp")
		animator->Play(L"IdleUp");
	else if (mDirection[2] == true && name == L"WalkDown")
		animator->Play(L"IdleDown");
	else if (mDirection[1] == true && name == L"WalkRight")
		animator->Play(L"IdleRight");
	else if (mDirection[3] == true && name == L"WalkLeft")
		animator->Play(L"IdleLeft");


}

void Player::Move()
{
	Animator* animator = GetComponent<Animator>();
	std::wstring& name = animator->GetPlayAnimation()->AnimationName();
	UINT index = GetDirection();

	wstring playName = L"Walk";
	playName += std::to_wstring(index);

	if (playName == name)
		return;

	animator->Play(playName);

	/*if (mDirection[0] == true && name != L"WalkUp")
		animator->Play(L"WalkUp");
	else if (mDirection[2] == true && name != L"WalkDown")
		animator->Play(L"WalkDown");
	else if (mDirection[1] == true && name != L"WalkRight")
		animator->Play(L"WalkRight");
	else if (mDirection[3] == true && name != L"WalkLeft")
		animator->Play(L"WalkLeft");*/


}

void Player::Attack()
{
	Animator* animator = GetComponent<Animator>();
	std::wstring& name = animator->GetPlayAnimation()->AnimationName();

	if (name.find(L"Attack") == wstring::npos)
	{
		if (GetDirection() == 1)
			animator->Play(L"AttackUp", false);
		else if (GetDirection() == 2)
			animator->Play(L"AttackRight", false);
		else if (GetDirection() == 3)
			animator->Play(L"AttackDown", false);
		else if (GetDirection() == 4)
			animator->Play(L"AttackLeft", false);
	}
	else
	{
		if (animator->GetPlayAnimation()->IsComplete())
		{
			mState = State::Idle;

			if (GetDirection() == 1)
				animator->Play(L"IdleUp");
			else if (GetDirection() == 2)
				animator->Play(L"IdleRight");
			else if (GetDirection() == 3)
				animator->Play(L"IdleDown");
			else if (GetDirection() == 4)
				animator->Play(L"IdleLeft");
		}
	}
}

void Player::SKil()
{
	Animator* animator = GetComponent<Animator>();
	std::wstring& name = animator->GetPlayAnimation()->AnimationName();

	if (name.find(L"Skil") == wstring::npos)
	{
		if (GetDirection() == 1)
			animator->Play(L"SkilUp", false);
		else if (GetDirection() == 2)
			animator->Play(L"SkilRight", false);
		else if (GetDirection() == 3)
			animator->Play(L"SkilDown", false);
		else if (GetDirection() == 4)
			animator->Play(L"SkilLeft", false);
	}
	else
	{
		if (animator->GetPlayAnimation()->IsComplete())
		{
			mState = State::Idle;

			if (GetDirection() == 1)
				animator->Play(L"IdleUp");
			else if (GetDirection() == 2)
				animator->Play(L"IdleRight");
			else if (GetDirection() == 3)
				animator->Play(L"IdleDown");
			else if (GetDirection() == 4)
				animator->Play(L"IdleLeft");
		}
	}
}

void Player::Hit()
{

}
