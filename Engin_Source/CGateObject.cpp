#include "CGateObject.h"
#include "CAnimator.h"
#include "CCollider2D.h"
#include "CResourceManager.h"
#include "CSpriteRenderer.h"
#include "CSceneManager.h"
#include "CGateCollider.h"
#include "CLoaddingScene.h"

GateObject::GateObject()
	: GameObject()
	, mNextScene(eSceneType::Endding)
	, mbNext(false)
{
 
}

GateObject::~GateObject()
{
}

void GateObject::Initalize()
{
	// 사이즈
	GetComponent<Transform>()->SetSize(Vector3(350.f, 350.f, 1.0f));

	std::weak_ptr<Texture2D> LedGatetex = ResourceManager::GetInstance()->Load<Texture2D>(L"LedGate", L"..//Resource//TP//LedGate.png");
	std::weak_ptr<Texture2D> BlueGatetex = ResourceManager::GetInstance()->Load<Texture2D>(L"BlueGate", L"..//Resource//TP//BlueGate.png");
	// 애니메이션
	{
		Animator* animator = AddComponent<Animator>();
		animator->Create(L"LedGate", LedGatetex, Vector2::Zero, Vector2(119.f, 159.f), Vector2::Zero, 15, 0.05f);
		animator->Create(L"BlueGate", BlueGatetex, Vector2::Zero, Vector2(188.f, 202.f), Vector2::Zero, 15, 0.05f);

		animator->Play(L"LedGate", true);
	}


	// 렌더러
	SpriteRenderer* sr = AddComponent<SpriteRenderer>();
	mMaterial = std::make_shared<Material>();
	mMaterial->SetRenderingMode(graphics::eRenderingMode::Transparent);

	std::weak_ptr<Mesh> mesh = ResourceManager::GetInstance()->Find<Mesh>(L"RectMesh");
	std::weak_ptr<Shader> shader = ResourceManager::GetInstance()->Find<Shader>(L"SpriteShader");

	mMaterial->SetShader(shader);

	sr->SetMesh(mesh);
	sr->SetMaterial(mMaterial);

	// 콜리더
	GateCollider* col = AddComponent<GateCollider>();
	col->SetType(eColliderType::Rect);
	col->SetSize(Vector2(0.5f, 0.5f));
}

void GateObject::Update()
{
	GameObject::Update();
}

void GateObject::FixedUpdate()
{
	GameObject::FixedUpdate();
}

void GateObject::Render()
{
	GameObject::Render();
}

void GateObject::NextScene()
{
	if (mbNext == false)
	{
		Scene* scene = SceneManager::GetInstance()->GetScene(eSceneType::Loadding);
		LoaddingScene* load = dynamic_cast<LoaddingScene*>(scene);
		if (load == nullptr)
			return;

		load->SeteSceneType(mNextScene);

		SceneManager::GetInstance()->LoadScene(eSceneType::Loadding);
	}
	else
	{
		SceneManager::GetInstance()->LoadScene(mNextScene);
	}

}
