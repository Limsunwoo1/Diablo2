#include "CPlayer.h"
#include "CSpriteRenderer.h"
#include "CMaterial.h"
#include "CTexture2D.h"
#include "CAnimator.h"
#include "CObject.h"
#include "CSceneManager.h"
#include "CAStar.h"
#include "CPlayerSelectButton.h"
#include "CTime.h"
#include "CUIManager.h"
#include "CSkilTreePanel.h"
#include "CAudioSource.h"
#include "CAudioClip.h"
#include "CAudioListner.h"


#include "CResourceManager.h"
#include "CServerManager.h"
#include "CObjectManager.h"
#include "CWorldManager.h"

using namespace graphics;

Player::Player()
	: GameObject()
	, mHP(10)
	, mMaxHP(500)
	, mMP(10)
	, mMaxMP(300)
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
	, mbDummyPlayer(false)
{
	SetName(L"Player");

	GetComponent<Transform>()->SetSize(Vector3(300.f, 300.f, 1.0f));

}


Player::Player(bool dummy)
	: GameObject()
	, mHP(10)
	, mMaxHP(500)
	, mMP(10)
	, mMaxMP(300)
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
	GetComponent<Transform>()->SetSize(Vector3(300.f, 300.f, 1.0f));
}

Player::~Player()
{
	if (mShadow)
		delete mShadow;

	mShadow = nullptr;
}

void Player::Initalize()
{
	mDirection.reset();
	mDirection[2] = 1;

	AStar* astart = AddComponent<AStar>();

	GameObject::Initalize();

	// Shdow Init
	mShadow = new GameObject;

	MeshRenderer* mr = mShadow->AddComponent<MeshRenderer>();
	std::weak_ptr<Mesh> mesh = ResourceManager::GetInstance()->Find<Mesh>(L"RectMesh");
	std::weak_ptr<Material> material = ResourceManager::GetInstance()->Find<Material>(L"ShadowMaterial");
	std::weak_ptr<Texture2D> tex = ResourceManager::GetInstance()->Find<Texture2D>(L"Shadow");
	material.lock()->SetTexture(eTextureSlot::T0, tex);

	mr->SetMesh(mesh);
	mr->SetMaterial(material);


	AudioSource* Audio = AddComponent<AudioSource>();
}

void Player::Update()
{
	if (mHP <= 0)
	{
		SetHP(1.f);
	}

	if (!IsDead())
	{
		mHP += Time::GetInstance()->DeltaTime() * 8.f;
		mMP += Time::GetInstance()->DeltaTime() * 10.f;

		if (mHP > mMaxHP)
			mHP = mMaxHP;

		if (mMP > mMaxMP)
			mMP = mMaxMP;
	}

	GameObject::Update();
	if (mShadow)
		mShadow->Update();

	Transform* shadowTr = mShadow->GetComponent<Transform>();
	Transform* OwnerTr = GetComponent<Transform>();

	Vector3 OwnerPos = OwnerTr->GetPosition();
	Vector3 OwnerSize = OwnerTr->GetSize();

	float shadowY = OwnerPos.y - (OwnerSize.y * 0.15f);

	Vector3 ShadowPos = shadowTr->GetPosition();
	ShadowPos = OwnerPos;
	ShadowPos.y = shadowY;

	shadowTr->SetPosition(ShadowPos);
	OwnerSize.x = OwnerSize.x * 0.15f;
	OwnerSize.y = OwnerSize.y * 0.075f;
	shadowTr->SetSize(OwnerSize);


	if (mbDummyPlayer)
		return;

	Run();
}

void Player::FixedUpdate()
{
	GameObject::FixedUpdate();
	if (mShadow)
		mShadow->FixedUpdate();

	if (mbDummyPlayer)
		return;

	mSendPosDelta += GETSINGLE(Time)->DeltaTime();

	if (mSendPosDelta > 0.01f)
	{
		mSendPosDelta = 0.0f;


		float intervalDelta = 0.0f;

		/*if (this->GameObject::GetState() == eState::dead)
			return;*/

		intervalDelta += GETSINGLE(Time)->DeltaTime();
		if (intervalDelta >= 0.01f)
		{
			intervalDelta = 0.0f;


			Server::Position_Packet packet = {};
			packet.type = Server::ServerDataType::PositionData;
			Math::Vector3 pos = this->GetComponent<Transform>()->GetPosition();
			packet.position = Server::Vec3(pos.x, pos.y, 0.0f);
			packet.sock = GETSINGLE(Server::ServerManager)->GetSocket();

			Animator* animator = GetComponent<Animator>();
			wstring AniName = animator->GetPlayAnimation()->AnimationName();
			packet.animationName = string(AniName.begin(), AniName.end());
			packet.animationIdx = animator->GetPlayAnimation()->GetIndex();

			GETSINGLE(Server::ServerManager)->PushSend((void*)&packet);
		}
	}
}

void Player::Render()
{

	SpriteRenderer* spr = GetComponent<SpriteRenderer>();
	std::weak_ptr<Texture2D> texture = ResourceManager::GetInstance()->Find<Texture2D>(L"test");
	spr->GetMaterial().lock()->SetTexture(eTextureSlot::T0, texture);

	if (mShadow)
		mShadow->Render();

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

			animator->Create(name, texture, Vector2(0.0f, y * i), Vector2(x, y), Vector2(0.0f, 0.0f), 8, 0.2f);
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
		std::weak_ptr<Texture2D> tex = ResourceManager::GetInstance()->Load<Texture2D>(L"PlayerAttack", L"attack1.png");

		float x = 124.95f;
		float y = 78.8125f;

		for (int i = 0; i < mDirection.size(); ++i)
		{
			wstring name = L"Attack";
			name += std::to_wstring(i);

			animator->Create(name, tex, Vector2(0.0f, y * i), Vector2(x, y), Vector2(0.0f, 0.0f), 20, 0.02f);

		}
		// 124.95  78.8125
	}

	{
		std::weak_ptr<Texture2D> tex = ResourceManager::GetInstance()->Load<Texture2D>(L"PlayerSkil", L"skill1.png");

		float x = 88.7857142857f;
		float y = 91.9375f;

		for (int i = 0; i < mDirection.size(); ++i)
		{
			wstring name = L"Skil";
			name += std::to_wstring(i);

			animator->Create(name, tex, Vector2(0.0f, y * i), Vector2(x, y), Vector2(0.0f, 0.0f), 14, 0.03f);

		}

		// 88.7857142857 91.9375
	}

	SpriteRenderer* spr = this->AddComponent<SpriteRenderer>();
	std::weak_ptr<Mesh> mesh = ResourceManager::GetInstance()->Find<Mesh>(L"RectMesh");
	std::weak_ptr<Material> material = ResourceManager::GetInstance()->Find<Material>(L"SpriteMaterial");

	spr->SetMesh(mesh);
	spr->SetMaterial(material);
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
	return true;
}

UINT Player::GetDirection()
{
	// index Ž��
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

	if (mode)
	{

	}
	else
	{
		GetComponent<AudioSource>()->Stop();
	}

	SetRunSpeed(mbRunMode);
}

void Player::SetRunSpeed(bool mode)
{
	// �ȱ� 2, �ٱ� 4

	UINT speed = mode == true ? 3 : 2;
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

		AudioSource* audio = GetComponent<AudioSource>();
		std::weak_ptr<AudioClip> clip = ResourceManager::GetInstance()->Load<AudioClip>(L"playerAttackSound", L"SoundResource\\attack.wav");
		audio->SetClip(clip);
		audio->SetLoop(false);
		audio->Play(0.03f);
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

void Player::LevelUp()
{
	mLevel++;

	SkilTreePanel* panel = nullptr;
	panel = UIManager::GetInstance()->GetUiInstance<SkilTreePanel>(L"SkilTree");
	if (panel == nullptr)
		return;

	panel->UpPoint();
	panel->UpPoint();

	//mMaxExp += mMaxExp * 0.2f;
}