#include "CPlayer.h"
#include "CResourceManager.h"
#include "CSpriteRenderer.h"
#include "CMaterial.h"
#include "CTexture2D.h"
#include "CAnimator.h"
#include "CObject.h"
#include "CSceneManager.h"
#include "CAStar.h"
#include "CWorldManager.h"
#include "CObjectManager.h"
#include "CPlayerSelectButton.h"

using namespace graphics;

Player::Player()
	: GameObject()
	, mHP(10)
	, mMaxHP(50)
	, mMP(10)
	, mMaxMP(50)
	, mDamege(5)
	, mIndex(0)
	, mbRunMode(false)
	, mMaxRunTime(10.f)
	, mRunTime(10.f)
	, mRunSpeed(2)
	, mState(PlayerState::Idle)
	, mCharType((UINT)eCharType::Sorceress)
	, mExp(0.0f)
	, mMaxExp(20.f)
	, mLevel(1)
	, mSaveName("sunwoo")
{
	SetName(L"Player");
}

Player::~Player()
{

}

void Player::Initalize()
{
	mDirection.reset();
	mDirection[2] = 1;

	AStar* astart = AddComponent<AStar>();

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
	Renderer::PlayerDataCB info = {};
	info.RunGauge = mRunTime / mMaxRunTime;
	info.hpGauge = mHP / mMaxHP;
	info.mpGauge = mMP / mMaxMP;
	//info.hpGauge = ;

	ConstantBuffer* cb = Renderer::constantBuffers[(UINT)eCBType::PlayerData];
	cb->SetData(&info);

	cb->Bind(eShaderStage::ALL);

	SpriteRenderer* spr = GetComponent<SpriteRenderer>();
	std::weak_ptr<Texture2D> texture = ResourceManager::GetInstance()->Find<Texture2D>(L"test");
	spr->GetMaterial().lock()->SetTexture(eTextureSlot::T0 ,texture);


	GameObject::Render();
}

void Player::InitAnimation()
{
	Animator* animator = this->AddComponent<Animator>();
	{
		std::weak_ptr<Texture2D> texture = ResourceManager::GetInstance()->Find<Texture2D>(L"PlayerWalk");

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

	// Run
	{
		std::weak_ptr<Texture2D> texture = ResourceManager::GetInstance()->Find<Texture2D>(L"PlayerRun");

		/*animator->Create(L"Walk0", texture, Vector2(0.0f, 0.0f), Vector2(60.f, 75.875f), Vector2(0.0f, 0.0f), 8, 0.1f);
		animator->Create(L"WalkUp", texture, Vector2(0.0f, 75.875 * 8), Vector2(60.f, 75.875f), Vector2(0.0f, 0.0f), 8, 0.1f);*/

		float x = 61.f;
		float y = 75.625f;

		for (int i = 0; i < mDirection.size(); ++i)
		{
			wstring name = L"Run";
			name += std::to_wstring(i);

			animator->Create(name, texture, Vector2(0.0f, y * i), Vector2(x, y), Vector2(0.0f, 0.0f), 8, 0.1f);
		}
		//61 75.625
	}

	{
		std::weak_ptr<Texture2D> texture = ResourceManager::GetInstance()->Find<Texture2D>(L"PlayerIdle");

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

			animator->Create(name, texture, Vector2(0.0f, y * i), Vector2(x, y), Vector2(0.0f, 0.0f), 20, 0.03f);

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

			animator->Create(name, texture, Vector2(0.0f, y * i), Vector2(x, y), Vector2(0.0f, 0.0f), 14, 0.03f);

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
	if (index >= mDirection.size())
		return false;

	mDirection.reset();
	mDirection[index] = 1;
	mIndex = index;

	std::cout << index << std::endl;
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

void Player::ChangeRunMode()
{
	bool mode = mbRunMode == true ? false : true;
	mbRunMode = mode;

	SetRunSpeed(mbRunMode);
}

void Player::SetRunSpeed(bool mode)
{
	// °È±â 2, ¶Ù±â 4

	UINT speed = mode == true ? 4 : 2 ;
	mRunSpeed = speed;
}

void Player::ActiveSkilFireBall()
{
}

void Player::SetState(PlayerState state)
{
	if (state == mState)
		return;

	mState = state;
}

void Player::Run()
{
	switch (mState)
	{
	case Player::PlayerState::Idle:
		Idle();
		break;
	case Player::PlayerState::Move:
		Move();
		break;
	case Player::PlayerState::Attack:
		Attack();
		break;
	case Player::PlayerState::Skil:
		SKil();
		break;
	case Player::PlayerState::Hit:
		Hit();
		break;
	default:
		break;
	}
}

void Player::Idle()
{
	Animator* animator = this->GetComponent<Animator>();
	std::wstring& name = animator->GetPlayAnimation()->AnimationName();
	
	wstring playName = L"Idle";
	playName += std::to_wstring(mIndex);

	if (playName == name)
		return;

	animator->Play(playName);
}

void Player::Move()
{
	Animator* animator = GetComponent<Animator>();
	std::wstring& name = animator->GetPlayAnimation()->AnimationName();
	UINT index = GetDirection();

	if (mbRunMode == false)
	{
		wstring playName = L"Walk";
		playName += std::to_wstring(index);

		if (playName == name)
			return;

		animator->Play(playName);
	}
	else if (mbRunMode == true)
	{
		wstring playName = L"Run";
		playName += std::to_wstring(index);

		if (playName == name)
			return;

		animator->Play(playName);
	}

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

	wstring playName = L"Attack";
	playName += std::to_wstring(mIndex);


	if (name.find(L"Attack") == wstring::npos)
	{
		animator->Play(playName, false);
	}
	else
	{
		if (animator->GetPlayAnimation()->IsComplete())
		{
			mState = PlayerState::Idle;
		}
	}
}

void Player::SKil()
{
	Transform* tr = GetComponent<Transform>();

	Animator* animator = GetComponent<Animator>();
	std::wstring& name = animator->GetPlayAnimation()->AnimationName();

	wstring playName = L"Skil";
	playName += std::to_wstring(mIndex);

	if (name.find(L"Skil") == wstring::npos)
	{
		animator->Play(playName, false);
		//ActiveSkilFireBall();
	}
	else
	{
		if (animator->GetPlayAnimation()->IsComplete())
		{
			mState = PlayerState::Idle;
		}
	}
}

void Player::Hit()
{

}
