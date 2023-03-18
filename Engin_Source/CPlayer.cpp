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

		animator->Create(L"WalkDown", texture, Vector2(0.0f, 0.0f), Vector2(60.f, 75.875f), Vector2(0.0f, 0.0f), 8, 0.1f);
		animator->Create(L"WalkLeft", texture, Vector2(0.0f, 75.875 * 3), Vector2(60.f, 75.875f), Vector2(0.0f, 0.0f), 8, 0.1f);
		animator->Create(L"WalkUp", texture, Vector2(0.0f, 75.875 * 8), Vector2(60.f, 75.875f), Vector2(0.0f, 0.0f), 8, 0.1f);
		animator->Create(L"WalkRight", texture, Vector2(0.0f, 75.875 * 11), Vector2(60.f, 75.875f), Vector2(0.0f, 0.0f), 8, 0.1f);

		//60 75.875
	}

	{
		std::shared_ptr<Texture2D> texture = std::make_shared<Texture2D>();
		texture->Load(L"Diablo2_Idle.png");
		ResourceManager::GetInstance()->Insert(L"PlayerIdle", texture);

		animator->Create(L"IdleDown", texture, Vector2(0.0f, 0.0f), Vector2(66.f, 72.f), Vector2(0.0f, 0.0f), 8, 0.1f);
		animator->Create(L"IdleLeft", texture, Vector2(0.0f, 72.f * 3), Vector2(66.f, 72.f), Vector2(0.0f, 0.0f), 8, 0.1f);
		animator->Create(L"IdleUp", texture, Vector2(0.0f, 72.f * 8), Vector2(66.f, 72.f), Vector2(0.0f, 0.0f), 8, 0.1f);
		animator->Create(L"IdleRight", texture, Vector2(0.0f, 72.f * 11), Vector2(66.f, 72.f), Vector2(0.0f, 0.0f), 8, 0.1f);

		// 66  72
	}

	{
		std::shared_ptr<Texture2D> texture = std::make_shared<Texture2D>();
		texture->Load(L"attack1.png");
		ResourceManager::GetInstance()->Insert(L"PlayerAttack", texture);

		animator->Create(L"AttackDown", texture, Vector2(0.0f, 0.0f), Vector2(124.95f, 78.8125f), Vector2(0.0f, 0.0f), 20, 0.03f);
		animator->Create(L"AttackLeft", texture, Vector2(0.0f, 78.8125f * 3), Vector2(124.95f, 78.8125f), Vector2(0.0f, 0.0f), 20, 0.03f);
		animator->Create(L"AttackUp", texture, Vector2(0.0f, 78.8125f * 8), Vector2(124.95f, 78.8125f), Vector2(0.0f, 0.0f), 20, 0.03f);
		animator->Create(L"AttackRight", texture, Vector2(0.0f, 78.8125f * 11), Vector2(124.95f, 78.8125f), Vector2(0.0f, 0.0f), 20, 0.03f);

		// 124.95  78.8125
	}

	{
		std::shared_ptr<Texture2D> texture = std::make_shared<Texture2D>();
		texture->Load(L"skill1.png");
		ResourceManager::GetInstance()->Insert(L"PlayerSKil", texture);

		animator->Create(L"SkilDown", texture, Vector2(0.0f, 0.0f), Vector2(88.7857142857f, 91.9375f), Vector2(0.0f, 0.0f), 14, 0.1f);
		animator->Create(L"SkilLeft", texture, Vector2(0.0f, 91.9375f * 3), Vector2(88.7857142857f, 91.9375f), Vector2(0.0f, 0.0f), 14, 0.1f);
		animator->Create(L"SkilUp", texture, Vector2(0.0f, 91.9375f * 8), Vector2(88.7857142857f, 91.9375f), Vector2(0.0f, 0.0f), 14, 0.1f);
		animator->Create(L"SkilRight", texture, Vector2(0.0f, 91.9375f * 11), Vector2(88.7857142857f, 91.9375f), Vector2(0.0f, 0.0f), 14, 0.1f);

		// 88.7857142857 91.9375
	}
}

void Player::PlayAnimation(const std::wstring& name)
{
	Animator* animator = this->GetComponent<Animator>();
	if(animator)
		animator->Play(name);
}

bool Player::PlayerDirection(int index)
{
	if (index - 1 > mDirection.size())
		return false;

	mDirection.reset();
	mDirection[index - 1] = 1;

	return true;
}

UINT Player::GetDirection()
{
	for (int i = 0; i < mDirection.size(); ++i)
	{
		if (mDirection[i] > 0)
			return i + 1;
	}
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

	if (mDirection[0] == true && name != L"WalkUp")
		animator->Play(L"WalkUp");
	else if(mDirection[2] == true && name != L"WalkDown")
		animator->Play(L"WalkDown");
	else if (mDirection[1] == true && name != L"WalkRight")
		animator->Play(L"WalkRight");
	else if (mDirection[3] == true && name != L"WalkLeft")
		animator->Play(L"WalkLeft");
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
