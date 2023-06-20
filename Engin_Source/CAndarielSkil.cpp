#include "CAndarielSkil.h"
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

AndarielSkil::AndarielSkil()
	: Skil()
	, mInterval(0.1f)
	, mDeleta(0.f)
	, mDeathTime(10.0f)
{
	mSpecialCastSkil.resize(9);
}

AndarielSkil::~AndarielSkil()
{
	for (BoltBase* obj : mSpecialCastSkil)
	{
		if (obj == nullptr)
			continue;

		delete obj;
		obj = nullptr;
	}
}

void AndarielSkil::Initalize()
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
		Load<Texture2D>(L"AndarielSpel", L"..//Resource//Monster//Andariel//AndarielSpel.png");

	std::weak_ptr<Shader> shader = ResourceManager::GetInstance()->Find<Shader>(L"SpriteShader");
	mMaterial->SetShader(shader);
	mMaterial->SetRenderingMode(graphics::eRenderingMode::Transparent);

	for (int i = 0; i < mSpecialCastSkil.size(); ++i)
	{
		mSpecialCastSkil[i] = new BoltBase();
		mSpecialCastSkil[i]->SetRun(true);
		mSpecialCastSkil[i]->Paused();
		mSpecialCastSkil[i]->SetSpeed(600.f);

		mSpecialCastSkil[i]->GetComponent<Transform>()->SetSize(Vector3(550.f, 550.f, 1.0f));
		mSpecialCastSkil[i]->SetDamege(40.f);

		Collider2D* col = mSpecialCastSkil[i]->AddComponent<Collider2D>();
		col->SetType(eColliderType::Rect);
		col->SetSize(Vector2(0.13f, 0.13f));

		mSpecialCastSkil[i]->Initalize();
		SpriteRenderer* sr = mSpecialCastSkil[i]->AddComponent<SpriteRenderer>();
		sr->SetMesh(mesh);
		sr->SetMaterial(mMaterial);

		Animator* animator = mSpecialCastSkil[i]->AddComponent<Animator>();
		animator->Create(L"AndarielSpel", tex, Vector2::Zero, Vector2(65.f, 65.f), Vector2::Zero, 24, 0.2f);
		animator->Play(L"AndarielSpel");
	}

	InitAnimation();
}

void AndarielSkil::Update()
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
		int cout = 0;
		int diffDegree = 15;
		for (BoltBase* obj : mSpecialCastSkil)
		{
			if (obj == nullptr)
				continue;

			if (obj->GetState() == eState::paused)
			{
				obj->Active();

				// 포지션이랑 앵글 설정 해야함
				// 각도 5 도
				Transform* objtr = obj->GetComponent<Transform>();
				Transform* Tr = GetComponent<Transform>();
				Vector3 Pos = Tr->GetPosition();

				objtr->SetPosition(Pos);

				GameObject* player = WorldManager::GetInstance()->GetPlayer();

				if (player == nullptr)
					continue;

				Transform* playerTr = player->GetComponent<Transform>();
				Vector3 PlayerPos = playerTr->GetPosition();
				Vector3 diff = PlayerPos - Pos;

				float radian = XMConvertToRadians(-45 + (cout * diffDegree));

				int radius = 100.f;
				int x = cosf(radian) * radius;
				int y = sinf(radian) * radius;


				if (diff.x < 0)
					x *= -1.f;
				if (diff.y < 0)
					y *= -1.f;

				Vector3 DurationPos = Pos;
				DurationPos.x += x;
				DurationPos.y += y;

				obj->Angle(Vector2(DurationPos.x, DurationPos.y));
				break;
			}
			cout++;
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

void AndarielSkil::FixedUpdate()
{
	Skil::FixedUpdate();

	for (GameObject* obj : mSpecialCastSkil)
	{
		if (obj == nullptr)
			continue;

		obj->FixedUpdate();
	}
}

void AndarielSkil::Render()
{
	Skil::Render();

	for (GameObject* obj : mSpecialCastSkil)
	{
		if (obj == nullptr)
			continue;

		obj->Render();
	}
}

void AndarielSkil::InitAnimation()
{

}
