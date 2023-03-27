#include "CSkilFireBall.h"
#include "CAnimator.h"
#include "CCollider2D.h"
#include "CSpriteRenderer.h"
#include "CMaterial.h"
#include "CMesh.h"
#include "CResourceManager.h"

SkilFireBall::SkilFireBall(Player* player)
	: SkilFire(player)
{
}

SkilFireBall::~SkilFireBall()
{
}

void SkilFireBall::Initalize()
{
	SkilFire::Initalize();

	AddComponent<Animator>();

	// collider
	Collider2D* col = AddComponent<Collider2D>();
	col->SetType(eColliderType::Rect);

	// renderer
	SpriteRenderer* sr = AddComponent<SpriteRenderer>();
	std::shared_ptr<Mesh> mesh = ResourceManager::GetInstance()->Find<Mesh>(L"RectMesh");
	std::shared_ptr<Material> material = ResourceManager::GetInstance()->Find<Material>(L"SpriteMaterial");
	sr->SetMesh(mesh);
	sr->SetMaterial(material);
}

void SkilFireBall::Update()
{
	SkilFire::Update();
}

void SkilFireBall::FixedUpdate()
{
	SkilFire::FixedUpdate();
}

void SkilFireBall::Render()
{
	SkilFire::Render();
}

void SkilFireBall::InitAnimation()
{
	Animator* animator = GetComponent<Animator>();

	{
		std::shared_ptr<Texture2D> texture = std::make_shared<Texture2D>();
		texture->Load(L"FireBall.png");
		ResourceManager::GetInstance()->Insert(L"FireBall", texture);

		float x = 96.92857142f;
		float y = 96.9375f;

		for (int i = 0; i < 16; ++i)
		{
			wstring name = L"FireBall";
			name += std::to_wstring(i);

			animator->Create(name, texture, Vector2(0.0f, y * i), Vector2(x, y), Vector2(0.0f, 0.0f), 14, 0.05f);

		}

		// 96.92857142  96.9375
	}

	SkilFireBall::InitAnimation();
}
