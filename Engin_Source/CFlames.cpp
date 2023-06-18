#include "CFlames.h"
#include "CSpriteRenderer.h"
#include "CAnimator.h"

#include "CResourceManager.h"
#include "CCollisionManager.h"
#include "CMonster.h"
#include "CCollider2D.h"
#include "CLayer.h"
#include "CSceneManager.h"


Flames::Flames()
	: Skil()
{
}

Flames::~Flames()
{
}

void Flames::Initalize()
{
	InitAnimation();

	// 사이즈
	Transform* tr = GetComponent<Transform>();
	tr->SetScale(Vector3(4.0f, 3.0f, 1.0f));
	tr->SetRotation(Vector3(0.f, 0.f, 0.f));

	// 렌더러
	SpriteRenderer* sr = AddComponent<SpriteRenderer>();
	weak_ptr<Mesh> mesh = ResourceManager::GetInstance()->Find<Mesh>(L"HalfAlphaMesh");
	weak_ptr<Material> material = ResourceManager::GetInstance()->Find<Material>(L"FlameMaterial");
	weak_ptr<Texture2D> tex = ResourceManager::GetInstance()->Find<Texture2D>(L"Flame");
	material.lock()->SetTexture(eTextureSlot::T0, tex);
	tex = ResourceManager::GetInstance()->Load<Texture2D>(L"FlameBoom", L"Meteor//MeteorBoom.png");
	material.lock()->SetTexture(eTextureSlot::T2, tex);
	
	sr->SetMesh(mesh);
	sr->SetMaterial(material);

	Collider2D* col = AddComponent<Collider2D>();
	col->SetType(eColliderType::Rect);
	col->SetSize(Vector2(0.25f, 0.25f));
	col->SetCenter(Vector2(0.f, -30.f));

	Death();
}

void Flames::Update()
{
	Skil::Update();

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
			}
		}
	}
}

void Flames::FixedUpdate()
{
	Skil::FixedUpdate();
}

void Flames::Render()
{
	Skil::Render();
}

void Flames::InitAnimation()
{
	weak_ptr<Texture2D> tex =
		ResourceManager::GetInstance()->Load<Texture2D>(L"Flame", L"Meteor//FlamesSprite2.png");

	Animator* animator = AddComponent<Animator>();
	animator->Create(L"Flame", tex, Vector2::Zero, Vector2(100.f, 100.f), Vector2::Zero, 20, 0.1f);

	tex = ResourceManager::GetInstance()->Load<Texture2D>(L"FlameBoom", L"Meteor//MeteorBoom.png");
	animator->Create(L"FlameBoom", tex, Vector2::Zero, Vector2(100.f, 100.f), Vector2::Zero, 19, 0.1f);

	animator->Play(L"FlameBoom", false);
}

void Flames::HitSkil(GameObject* obj)
{
	Monster* monster = dynamic_cast<Monster*>(obj);

	if (monster == nullptr)
		return;

	monster->SetMonsterStatusEffect(eElementType::HitFire);
}