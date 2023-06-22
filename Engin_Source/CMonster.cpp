#include "CMonster.h"
#include "CTime.h"
#include "CAnimator.h"
#include "CLayer.h"
#include "CSceneManager.h"
#include "CScript.h"
#include "CMeshRenderer.h"
#include "CResourceManager.h"
#include "CTexture2D.h"
#include "CCollider2D.h"
#include "Cplayer.h"
#include "CWorldManager.h"
#include "CInput.h"
#include "CCollisionManager.h"
#include "Cplayer.h"

#include <iostream>
#include <stdlib.h>
#include <time.h>

#include "CItemBase.h"
#include "CCapItem.h"
#include "CGloveItem.h"
#include "CHpPotionItem.h"
#include "CMpPotionItem.h"
#include "CShoesItem.h"
#include "CBeltItem.h"
#include "CWeponItem.h"
#include "CSuitItem.h"

#include "CItemManager.h"
#include "CAudioSource.h"

#include <random>


std::random_device random;
std::mt19937 gen(random());
std::uniform_int_distribution<int> type(0, 8);

Monster::Monster()
	: GameObject()
	, mMonsterState(MonsterState::Idle)
	, mMonsterStatusEffect(eElementType::None)
	, mMaxHP(50.f)
	, mHP(50.f)
	, mIndex(4)
	, mDotDamageCoolTime(0.0f)
	, mDeltaTime(0.0f)
	, mReset(false)
	, mDirection{}
	, mSpawnPos(-1.f, -1.f)
	, mDamege(20.0f)
	, mLightElementDamege(mDamege * 0.5f)
	, mElementTime(3.0f)
	, AttackSize(Vector2(200.f,200.f))
{
	mDirection[mIndex] = 1;

	srand(time(NULL));
}

Monster::~Monster()
{
	if (mShadow)
		delete mShadow;

	mShadow = nullptr;
}

void Monster::Initalize()
{
	// Shdow Init
	mShadow = new GameObject;

	MeshRenderer* mr = mShadow->AddComponent<MeshRenderer>();
	std::weak_ptr<Mesh> mesh = ResourceManager::GetInstance()->Find<Mesh>(L"RectMesh");
	std::weak_ptr<Material> material = ResourceManager::GetInstance()->Find<Material>(L"ShadowMaterial");
	std::weak_ptr<Texture2D> tex = ResourceManager::GetInstance()->Find<Texture2D>(L"Shadow");
	material.lock()->SetTexture(eTextureSlot::T0, tex);

	mr->SetMesh(mesh);
	mr->SetMaterial(material);

	Collider2D* col = AddComponent<Collider2D>();
	col->SetType(eColliderType::Rect);
	col->SetSize(Vector2(0.25f, 0.6f));
}

void Monster::Update()
{
	if (GetHP() < 0)
	{
		SetMonsterState(MonsterState::Dead);
		SetMonsterStatusEffect(eElementType::None);
	}

	CheckPointCollision();


	if (GetMonsterState() == MonsterState::Dead)
	{
		Animator* animator = GetComponent<Animator>();
		wstring name = animator->GetPlayAnimation()->GetName();
		if (name.find(L"Death") != std::wstring::npos)
		{
			if (!animator->GetPlayAnimation()->IsComplete())
			{
				return;
			}

			Paused();
			mShadow->Paused();
		}
	}

	float time = Time::GetInstance()->DeltaTime();
	SetCurDeltaTime(time);
	StatusEffect();

	// 몬스터의 현제 상태에 따라 렌더 되는 방식을 정해줄 enum 값
	// 예) 얼음 상태이상일시 몬스터 푸른색으로 렌더
	Animator* animator = GetComponent<Animator>();
	animator->SetElementType((UINT)mMonsterStatusEffect);

	if(mTarget == nullptr)
		mReset = true;


	Scene* scene = SceneManager::GetInstance()->GetActiveScene();
	Layer& layer = scene->GetLayer(eLayerType::Monster);
	const std::vector<GameObject*>& Objects = layer.GetGameObjects();
	
	Vector2 collisionSize = Vector2(35.f, 35.f);

	for (GameObject* obj : Objects)
	{
		if (obj == nullptr)
			continue;

		if (obj == this)
			continue;

		if (obj->GetState() != eState::active)
			continue;

		if (GetMonsterState() == MonsterState::Dead)
			continue;

		Monster* monsterobj = dynamic_cast<Monster*>(obj);
		if (monsterobj)
		{
			if (monsterobj->GetMonsterState() == MonsterState::Dead)
				continue;
		}

		Transform* Tr = GetComponent<Transform>();
		Vector3 Pos = Tr->GetPosition();

		Transform* objTr = obj->GetComponent<Transform>();
		Vector3 objPos = objTr->GetPosition();

		if ((Pos.x + collisionSize.x) < (objPos.x - collisionSize.x) || (Pos.x - collisionSize.x) > (objPos.x + collisionSize.x))
			continue;

		if ((Pos.y + collisionSize.y) < (objPos.y - collisionSize.y) || (Pos.y - collisionSize.y) > (objPos.y + collisionSize.y))
			continue;


		Vector3 diff = objPos - Pos;
		diff.Normalize();
		objPos += diff * Time::GetInstance()->DeltaTime() * 300.f;
		Pos += -(diff) * Time::GetInstance()->DeltaTime() * 300.f;

		Tr->SetPosition(Pos);
		objTr->SetPosition(objPos);

		Script* sript = obj->GetScript<Script>();
		if (sript == nullptr)
			continue;

		sript->ResetAStar();
		break;
		//sript->SetTime(0.f);
	}

	Run();
	GameObject::Update();

	if (mShadow)
		mShadow->Update();

	Transform* shadowTr = mShadow->GetComponent<Transform>();
	Transform* OwnerTr = GetComponent<Transform>();

	Vector3 OwnerPos = OwnerTr->GetPosition();
	Vector3 OwnerSize = OwnerTr->GetSize();

	float shadowY = OwnerPos.y - (OwnerSize.y * 0.25f);

	Vector3 ShadowPos = shadowTr->GetPosition();
	ShadowPos = OwnerPos;
	ShadowPos.y = shadowY;

	shadowTr->SetPosition(ShadowPos);
	OwnerSize.x = OwnerSize.x * 0.15f;
	OwnerSize.y = OwnerSize.y * 0.075f;
	shadowTr->SetSize(OwnerSize);
}

void Monster::FixedUpdate()
{
	GameObject::FixedUpdate();

	if (mShadow)
		mShadow->FixedUpdate();
}

void Monster::Render()
{

	if (mShadow)
		mShadow->Render();

	
	GameObject::Render();
}

void Monster::StatusEffect()
{
	float time = Time::GetInstance()->DeltaTime();
	GetComponent<Animator>()->GetPlayAnimation()->SetTimeControl(false);
	if (mMonsterStatusEffect == eElementType::HitFrozen)
	{
		// 얼음 속성일때 모든 행동제약이 두배 느리게 이동
		float intime = 0.0f;
		Animation* animation = GetComponent<Animator>()->GetPlayAnimation();
		if (animation)
		{
			animation->SetTimeControl(true);
			intime = animation->GetTime();
			intime += time * 0.5f;
			animation->SetTime(intime);
			SetCurDeltaTime(time * 0.5f);
		}
	}
	else if (mMonsterStatusEffect == eElementType::HitFire)
	{
		mDotDamageCoolTime += Time::GetInstance()->DeltaTime();
		if (mDotDamageCoolTime >= 0.3f)
		{
			mDotDamageCoolTime -= 0.3f;
			int hp = GetHP();
			hp -= 1.f;
			SetHP(hp);
		}
	}
	else if (mMonsterStatusEffect == eElementType::HitLight)
	{
		GetComponent<Animator>()->GetPlayAnimation()->SetTimeControl(false);
		SetDamege(mLightElementDamege);
	}
}

void Monster::Run()
{
	switch (mMonsterState)
	{
	case Monster::MonsterState::Idle:		idle();			break;
	case Monster::MonsterState::Move:		move();			break;
	case Monster::MonsterState::Attack:		attack();		break;
	case Monster::MonsterState::Hit:		hit();			break;
	case Monster::MonsterState::Dead:		monsterDead();	break;
	default:												break;
	}

	if (mMonsterStatusEffect != eElementType::None)
		discountElement();

	switch (mMonsterStatusEffect)
	{
	case eElementType::HitFire:		hitFire();				break;
	case eElementType::HitFrozen:	hitFrozen();			break;
	case eElementType::HitLight:		hitLight();			break;
	default:												break;
	}
}

void Monster::CheckPointCollision()
{
	Vector2 mousePos = Input::GetInstance()->GetMouseWorldPos(true);

	bool colCheck = false;
	colCheck = CollisionManager::GetInstance()->AABBRect_VS_Point(GetComponent<Collider2D>(), mousePos);

	if (colCheck)
	{
		Input::GetInstance()->SetPicMonster(this);

		if (Input::GetInstance()->GetKeyDown(eKeyCode::LBTN))
		{
			Player* player = dynamic_cast<Player*>(WorldManager::GetInstance()->GetPlayer());

			if (player->GetState() == Player::PlayerState::Attack)
				return;

			float damege = player->GetDamege();
			mHP -= damege;

			player->SetState(Player::PlayerState::Attack);
		}
	}
}

void Monster::Attack()
{
	Player* player = dynamic_cast<Player*>(WorldManager::GetInstance()->GetPlayer());

	if (player == nullptr)
		return;

	Vector3 PlayerPos = player->GetComponent<Transform>()->GetPosition();
	Vector3 Pos = GetComponent<Transform>()->GetPosition();

	Vector2 vec = Pos - Vector2(PlayerPos.x, PlayerPos.y);
	Vector2 Vec1 = Vector2(0.0f, 0.0f);

	if (vec.x <= 0.f)
		Vec1 = Vector2(-1.0f, 0.0f);
	else
		Vec1 = Vector2(1.0f, 0.0f);

	Vec1.Normalize();
	vec.Normalize();

	float that = Vec1.Dot(vec);
	float radian = acos(that);

	int radius = 100.f;
	// 원의 방정식
	// https://nenara.com/68
	int x = cosf(radian) * radius;
	int y = sinf(radian) * radius;

	if (vec.x < 0)
		x *= -1.f;
	if (vec.y < 0)
		y *= -1.f;

	Vector3 SearchPos = PlayerPos;
	SearchPos.x += x;
	SearchPos.y += y;


	if (SearchPos.x + (AttackSize.x * 0.5f) < PlayerPos.x || SearchPos.x - (AttackSize.x * 0.5f) > PlayerPos.x)
		return;

	if (SearchPos.y + (AttackSize.y * 0.5f) < PlayerPos.y || SearchPos.y - (AttackSize.y * 0.5f) > PlayerPos.y)
		return;

	float hp = player->GetHP();
	hp -= GetDamege();

	player->SetHP(hp);
}

void Monster::DropItem()
{
	Player* player = nullptr;
	player = dynamic_cast<Player*>(WorldManager::GetInstance()->GetPlayer());
	if (player == nullptr)
		return;

	float exp = player->GetExp();
	exp += 50;

	float max = player->GetMaxExp();
	if (exp > max)
	{
		player->SetExp(exp - max);
		player->LevelUp();
	}

	////////////////////////////// 플레이어 레벨업
	ItemBase* item = nullptr;
	
	UINT itemType = type(gen);

	switch ((eEquipmentType)itemType)
	{
	case eEquipmentType::Belt:
	{
		item = new BeltItem(L"Belt");
		item->SetItemType(eEquipmentType::Belt);
	}
	break;

	case eEquipmentType::Cap:
	{
		item = new CapItem(L"Cap");
		item->SetItemType(eEquipmentType::Cap);
	}
	break;

	case eEquipmentType::Glove:
	{
		item = new GloveItem(L"Glove");
		item->SetItemType(eEquipmentType::Glove);
	}
	break;

	case eEquipmentType::HpPotion:
	{
		item = new HpPotionItem();

		item->SetItemType(eEquipmentType::HpPotion);
	}
	break;

	case eEquipmentType::MpPotion:
	{
		item = new MpPotionItem();
		item->SetItemType(eEquipmentType::MpPotion);
	}
	break;

	case eEquipmentType::Shoes:
	{
		item = new ShoesItem(L"Shoes");
		item->SetItemType(eEquipmentType::Shoes);
	}
	break;

	case eEquipmentType::Suit:
	{
		item = new SuitItem(L"Suit");
		item->SetItemType(eEquipmentType::Suit);
	}
	break;

	case eEquipmentType::Wepon:
	{
		item = new WeponItem(L"Suit");
		item->SetItemType(eEquipmentType::Wepon);
	}
	break;


	default:
		break;
	}

	if (item)
	{
		item->Initalize();
		item->Drop();
		item->SetStage(true);

		ItemManager::GetInstance()->AddItem(item);

		Transform* itemTr = item->GetComponent<Transform>();
		Transform* Tr = GetComponent<Transform>();

		itemTr->SetPosition(Tr->GetPosition());
		itemTr->SetSize(item->GetWorldSize());

		AudioSource* source = item->GetComponent<AudioSource>();
		source->Play();
	}
}

bool Monster::MonsterDirection(int index)
{
	if (index >= mDirection.size())
		return false;

	mDirection.reset();
	mDirection[index] = 1;
	mIndex = index;

	return true;
}

UINT Monster::GetDirection()
{
	return mIndex;
}

void Monster::discountElement()
{
	mElementTime -= Time::GetInstance()->DeltaTime();

	if (mElementTime <= 0.0f)
	{
		mElementTime = 3.0f;

		if (mMonsterStatusEffect == eElementType::HitLight)
		{
			SetDamege(mDamege);
		}

		mMonsterStatusEffect = eElementType::None;
	}
}

void Monster::hitFire()
{

}

void Monster::hitFrozen()
{

}

void Monster::hitLight()
{

}