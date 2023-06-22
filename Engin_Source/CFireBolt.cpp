#include "CFireBolt.h"
#include "CAnimator.h"
#include "CResourceManager.h"
#include "CTransform.h"
#include "CSpriteRenderer.h"
#include "CObject.h"
#include "CLight.h"
#include "CCollisionManager.h"
#include "CMonster.h"
#include "CCollider2D.h"
#include "CAudioSource.h"
#include "CAudioClip.h"
#include "CTime.h"

FireBolt::FireBolt()
	: BoltBase()
	, RunSound(false)
	, Delta(0.5f)
{
	SetDamege(10.f);
	SetElementType(eElementType::HitFire);
}

FireBolt::~FireBolt()
{
}

void FireBolt::Initalize()
{
	// 사이즈
	Transform* tr = GetComponent<Transform>();
	tr->SetSize(Vector3(550.f, 300.f, 1.0f));

	// 애니메이터 초기화
	InitAnimation();

	// 렌더러
	SpriteRenderer* sr = AddComponent<SpriteRenderer>();
	weak_ptr<Mesh> mesh = ResourceManager::GetInstance()->Find<Mesh>(L"RectMesh");
	weak_ptr<Material> material = ResourceManager::GetInstance()->Find<Material>(L"FireBoltMaterial");
	weak_ptr<Texture2D> tex = ResourceManager::GetInstance()->Find<Texture2D>(L"FireBolt");
	material.lock()->SetTexture(eTextureSlot::T0, tex);

	sr->SetMesh(mesh);
	sr->SetMaterial(material);

	Collider2D* col = AddComponent<Collider2D>();
	col->SetType(eColliderType::Rect);
	col->SetSize(Vector2(0.03f, 0.05f));
	col->SetCenter(Vector2(0.f, 0.f));

	//
	AudioSource* audio = AddComponent<AudioSource>();
	std::weak_ptr<AudioClip> clip = ResourceManager::GetInstance()->Load<AudioClip>(L"FireBlot", L"Sound\\1\\skill\\sorceress\\firebolt1.wav");
	audio->SetClip(clip);
	audio->SetLoop(false);
}

void FireBolt::Update()
{
	BoltBase::Update();

	if (GetRun() == false)
		return;

	if (!RunSound)
	{
		GetComponent<AudioSource>()->Play(0.3f);
		RunSound = true;
		return;
	}

	if (Delta >= 0.0f)
	{
		Delta -= Time::GetInstance()->DeltaTime();

		if (Delta < 0.0f)
		{
			GetComponent<AudioSource>()->Stop();
		}
	}

	Layer& layer = SceneManager::GetInstance()->GetActiveScene()->GetLayer(eLayerType::Monster);
	const std::vector<GameObject*>& Objects = layer.GetGameObjects();

	Collider2D* col = GetComponent<Collider2D>();
	if (col != nullptr)
	{
		for (GameObject* obj : Objects)
		{
			if (obj == nullptr)
				continue;

			Monster* monster = dynamic_cast<Monster*>(obj);
			if (monster == nullptr)
				continue;

			if (monster->GetMonsterState() == Monster::MonsterState::Dead)
				continue;

			bool collistion = false;
			collistion = CollisionManager::GetInstance()->AABBRect_VS_Rect(col, obj->GetComponent<Collider2D>());

			if (collistion)
			{
				HitSkil(obj);
				break;
			}
		}
	}

}

void FireBolt::FixedUpdate()
{
	BoltBase::FixedUpdate();
}

void FireBolt::Render()
{
	BoltBase::Render();
}

void FireBolt::InitAnimation()
{
	Animator* animator = AddComponent<Animator>();
	weak_ptr<Texture2D> tex =
		ResourceManager::GetInstance()->Load<Texture2D>(L"FireBolt", L"FirBolt//FirteBolt2.png");


	animator->Create(L"FireBolt", tex, Vector2::Zero, Vector2(100.f, 38.f), Vector2::Zero, 9, 0.1f);
	animator->Play(L"FireBolt");
}

void FireBolt::HitSkil(GameObject* obj)
{
	Monster* monster = dynamic_cast<Monster*>(obj);

	if (monster == nullptr)
		return;

	float hp = monster->GetHP();
	float damage = GetDamege();

	hp -= damage;
	monster->SetHP(hp);

	monster->SetMonsterStatusEffect(GetElementType());
	Death();
}