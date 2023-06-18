#include "CAndarielSkil.h"
#include "CBoltBase.h"
#include "CTime.h"
#include "CResourceManager.h"
#include "CAnimator.h"
#include "CTexture2D.h"
#include "CSpriteRenderer.h"
#include "CWorldManager.h"

AndarielSkil::AndarielSkil()
	: Skil()
	, mInterval(0.05f)
	, mDeleta(0.f)
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

		mSpecialCastSkil[i]->GetComponent<Transform>()->SetSize(Vector3(400.f, 400.f, 1.0f));


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
	Skil::Update();

	mDeleta += Time::GetInstance()->DeltaTime();

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

				obj->Angle(Vector2(PlayerPos.x, PlayerPos.y));
				break;
			}
		}
	}

	for (GameObject* obj : mSpecialCastSkil)
	{
		if (obj == nullptr)
			continue;

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
