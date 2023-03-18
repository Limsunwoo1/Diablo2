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

}

void Player::SKil()
{

}

void Player::Hit()
{

}
