#include "CFrozenBolt.h"
#include "CAnimator.h"
#include "CResourceManager.h"
#include "CTransform.h"
#include "CSpriteRenderer.h"
#include "CObject.h"
#include "CLight.h"
#include "CCollisionManager.h"
#include "CMonster.h"
#include "CCollider2D.h"

FrozenBolt::FrozenBolt()
	: BoltBase()
{
	SetElementType(eElementType::HitFrozen);
}

FrozenBolt::~FrozenBolt()
{
}

void FrozenBolt::Initalize()
{
	// ������
	Transform* tr = GetComponent<Transform>();
	tr->SetSize(Vector3(320.f, 320.f, 1.0f));

	// �ִϸ����� �ʱ�ȭ
	InitAnimation();

	// ������
	SpriteRenderer* sr = AddComponent<SpriteRenderer>();
	weak_ptr<Mesh> mesh = ResourceManager::GetInstance()->Find<Mesh>(L"RectMesh");
	weak_ptr<Material> material = ResourceManager::GetInstance()->Find<Material>(L"FrozenBoltMaterial");
	weak_ptr<Texture2D> tex = ResourceManager::GetInstance()->Find<Texture2D>(L"FrozenBolt");
	material.lock()->SetTexture(eTextureSlot::T0, tex);

	sr->SetMesh(mesh);
	sr->SetMaterial(material);

	// �浹ü
	Collider2D* col = AddComponent<Collider2D>();
	col->SetType(eColliderType::Rect);
	col->SetSize(Vector2(0.05f, 0.1f));
	col->SetCenter(Vector2(0.f, 0.f));
}

void FrozenBolt::Update()
{
	BoltBase::Update();

	if (GetRun() == false)
		return;

	Layer& layer = SceneManager::GetInstance()->GetActiveScene()->GetLayer(eLayerType::Monster);
	const std::vector<GameObject*>& Objects = layer.GetGameObjects();

	Collider2D* col = GetComponent<Collider2D>();
	if (col != nullptr)
	{
		for (GameObject* obj : Objects)
		{
			if (obj == nullptr)
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

void FrozenBolt::FixedUpdate()
{
	BoltBase::FixedUpdate();
}

void FrozenBolt::Render()
{
	BoltBase::Render();
}

void FrozenBolt::InitAnimation()
{
	Animator* animator = AddComponent<Animator>();

	weak_ptr<Texture2D> tex =
		ResourceManager::GetInstance()->Load<Texture2D>(L"FrozenBolt", L"FrozenBolt//FrozenBolt.png");

	animator->Create(L"FrozenBolt", tex, Vector2::Zero, Vector2(45.f, 59.f), Vector2::Zero, 5, 0.1f);
	animator->Play(L"FrozenBolt");
}

void FrozenBolt::HitSkil(GameObject* obj)
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