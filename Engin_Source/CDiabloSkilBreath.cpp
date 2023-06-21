#include "CDiabloSkilBreath.h"
#include "CBoltBase.h"
#include "CTime.h"
#include "CResourceManager.h"
#include "CAnimator.h"
#include "CTexture2D.h"
#include "CSpriteRenderer.h"
#include "CWorldManager.h"
#include "CObject.h"

#include "CCollisionManager.h"
#include "CMonster.h"
#include "CCollider2D.h"
#include "CLayer.h"
#include "CSceneManager.h"

#include "Cplayer.h"

DiabloSkilBreath::DiabloSkilBreath()
	: Skil()
	, mInterval(0.2f)
	, mDeleta(0.f)
	, mDeathTime(10.0f)
{
	mSpecialCastSkil.resize(5);
}

DiabloSkilBreath::~DiabloSkilBreath()
{
	for (BoltBase* obj : mSpecialCastSkil)
	{
		if (obj == nullptr)
			continue;

		delete obj;
		obj = nullptr;
	}
}

void DiabloSkilBreath::Initalize()
{
	{
		// 주체의 렌더러와 마터리얼이 없으면 렌더가 호출이 안된다
		SpriteRenderer* mSr = this->AddComponent<SpriteRenderer>();

		std::weak_ptr<Mesh> rectmesh = ResourceManager::GetInstance()->Find<Mesh>(L"RectMesh");
		std::weak_ptr<Material> material = ResourceManager::GetInstance()->Find<Material>(L"Rect2Material");
		mSr->SetMesh(rectmesh);
		mSr->SetMaterial(material);

		mSr->SetRenderStop(true);
	}



	mMaterial = std::make_shared<Material>();
	std::weak_ptr<Mesh> mesh = ResourceManager::GetInstance()->Find<Mesh>(L"RectMesh");
	std::weak_ptr<Texture2D> tex = ResourceManager::GetInstance()->
		Load<Texture2D>(L"DiabloLightnig", L"..//Resource//Monster//Diablo//DiabloLightnig.png");

	std::weak_ptr<Shader> shader = ResourceManager::GetInstance()->Find<Shader>(L"SpriteShader");
	mMaterial->SetShader(shader);
	mMaterial->SetRenderingMode(graphics::eRenderingMode::Transparent);

	for (int i = 0; i < mSpecialCastSkil.size(); ++i)
	{
		mSpecialCastSkil[i] = new BoltBase();
		mSpecialCastSkil[i]->SetRun(true);
		mSpecialCastSkil[i]->Paused();
		mSpecialCastSkil[i]->SetSpeed(760.f);

		mSpecialCastSkil[i]->GetComponent<Transform>()->SetSize(Vector3(200.f, 650.f, 1.0f));
		mSpecialCastSkil[i]->SetDamege(70.f);

		Collider2D* col = mSpecialCastSkil[i]->AddComponent<Collider2D>();
		col->SetType(eColliderType::Rect);
		col->SetSize(Vector2(0.05f, 0.1f));

		mSpecialCastSkil[i]->Initalize();
		SpriteRenderer* sr = mSpecialCastSkil[i]->AddComponent<SpriteRenderer>();
		sr->SetMesh(mesh);
		sr->SetMaterial(mMaterial);

		Animator* animator = mSpecialCastSkil[i]->AddComponent<Animator>();
		animator->Create(L"DiabloLightnig", tex, Vector2::Zero, Vector2(120.f, 157.f), Vector2::Zero, 15, 0.1f);
		animator->Play(L"DiabloLightnig");
	}

	InitAnimation();
}

void DiabloSkilBreath::Update()
{
	if (mDeathTime <= 0)
	{
		Object::ObjectDestroy(this);
		return;
	}

	Skil::Update();

	mDeleta += Time::GetInstance()->DeltaTime();
	mDeathTime -= Time::GetInstance()->DeltaTime();

	if (mDeleta >= mInterval)
	{
		mDeleta -= mInterval;
		for (BoltBase* obj : mSpecialCastSkil)
		{
			if (obj == nullptr)
				continue;

			if (obj->GetState() == eState::paused)
			{
				obj->Active();

				Transform* objtr = obj->GetComponent<Transform>();
				Transform* Tr = GetComponent<Transform>();
				Vector3 Pos = Tr->GetPosition();


				GameObject* player = WorldManager::GetInstance()->GetPlayer();

				if (player == nullptr)
					continue;
				Transform* playerTr = player->GetComponent<Transform>();
				Vector3 PlayerPos = playerTr->GetPosition();

				objtr->SetPosition(Pos);
				obj->Angle(Vector2(PlayerPos.x, PlayerPos.y));

				break;
			}
		}
	}

	for (BoltBase* obj : mSpecialCastSkil)
	{
		if (obj == nullptr)
			continue;

		if (obj->GetState() != eState::active)
			continue;

		Collider2D* objCol = obj->GetComponent<Collider2D>();
		Player* player = dynamic_cast<Player*>(WorldManager::GetInstance()->GetPlayer());
		Collider2D* playerCol = player->GetComponent<Collider2D>();

		if (CollisionManager::GetInstance()->AABBRect_VS_Rect(objCol, playerCol))
		{
			float hp = player->GetHP();
			hp -= obj->GetDamege();
			player->SetHP(hp);

			obj->Death();
			continue;
		}

		obj->Update();
	}
}

void DiabloSkilBreath::FixedUpdate()
{
	Skil::FixedUpdate();

	for (GameObject* obj : mSpecialCastSkil)
	{
		if (obj == nullptr)
			continue;

		obj->FixedUpdate();
	}
}

void DiabloSkilBreath::Render()
{
	Skil::Render();

	for (GameObject* obj : mSpecialCastSkil)
	{
		if (obj == nullptr)
			continue;

		obj->Render();
	}
}

void DiabloSkilBreath::InitAnimation()
{

}
