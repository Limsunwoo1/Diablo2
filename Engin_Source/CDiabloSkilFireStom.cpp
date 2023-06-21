#include "CDiabloSkilFireStom.h"
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

DiabloSkilFireStom::DiabloSkilFireStom()
	: Skil()
	, mDeleta(0.f)
	, mDeathTime(10.0f)
{
	mSpecialCastSkil.resize(20);
}

DiabloSkilFireStom::~DiabloSkilFireStom()
{
	for (BoltBase* obj : mSpecialCastSkil)
	{
		if (obj == nullptr)
			continue;

		delete obj;
		obj = nullptr;
	}
}

void DiabloSkilFireStom::Initalize()
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
		Load<Texture2D>(L"DiabloFirStom", L"..//Resource//Monster//Diablo//DiabloFirStom.png");

	std::weak_ptr<Shader> shader = ResourceManager::GetInstance()->Find<Shader>(L"SpriteShader");
	mMaterial->SetShader(shader);
	mMaterial->SetRenderingMode(graphics::eRenderingMode::Transparent);

	float degree = 360.f / mSpecialCastSkil.size();
	for (int i = 0; i < mSpecialCastSkil.size(); ++i)
	{
		mSpecialCastSkil[i] = new BoltBase();
		mSpecialCastSkil[i]->SetRun(true);
		mSpecialCastSkil[i]->Paused();
		mSpecialCastSkil[i]->SetSpeed(760.f);

		mSpecialCastSkil[i]->GetComponent<Transform>()->SetSize(Vector3(450.f, 450.f, 1.0f));
		mSpecialCastSkil[i]->SetDamege(30.f);

		Transform* Tr = mSpecialCastSkil[i]->GetComponent<Transform>();
		Tr->SetRotation(Vector3(0.0f, 0.0f, i * degree));


		Collider2D* col = mSpecialCastSkil[i]->AddComponent<Collider2D>();
		col->SetType(eColliderType::Rect);
		col->SetSize(Vector2(0.05f, 0.05f));

		mSpecialCastSkil[i]->Initalize();
		SpriteRenderer* sr = mSpecialCastSkil[i]->AddComponent<SpriteRenderer>();
		sr->SetMesh(mesh);
		sr->SetMaterial(mMaterial);

		Animator* animator = mSpecialCastSkil[i]->AddComponent<Animator>();
		animator->Create(L"DiabloFirStom", tex, Vector2::Zero, Vector2(96.f, 83.f), Vector2::Zero, 16, 0.05f);
		animator->Play(L"DiabloFirStom");
	}

	InitAnimation();
}

void DiabloSkilFireStom::Update()
{
	if (mDeathTime <= 0)
	{
		Object::ObjectDestroy(this);
		return;
	}

	Skil::Update();

	mDeleta += Time::GetInstance()->DeltaTime();
	mDeathTime -= Time::GetInstance()->DeltaTime();


	for (BoltBase* obj : mSpecialCastSkil)
	{
		if (obj == nullptr)
			continue;

		if (obj->GetState() == eState::paused)
		{
			obj->Active();
			Transform* ownerTr = GetComponent<Transform>();
			Transform* objTr = obj->GetComponent<Transform>();
			objTr->SetPosition(ownerTr->GetPosition());
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

void DiabloSkilFireStom::FixedUpdate()
{
	Skil::FixedUpdate();

	for (GameObject* obj : mSpecialCastSkil)
	{
		if (obj == nullptr)
			continue;

		obj->FixedUpdate();
	}
}

void DiabloSkilFireStom::Render()
{
	Skil::Render();

	for (GameObject* obj : mSpecialCastSkil)
	{
		if (obj == nullptr)
			continue;

		obj->Render();
	}
}

void DiabloSkilFireStom::InitAnimation()
{

}
