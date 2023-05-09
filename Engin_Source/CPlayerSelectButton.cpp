#include "CPlayerSelectButton.h"
#include "CGameObject.h"
#include "CSpriteRenderer.h"
#include "CAnimator.h"
#include "CResourceManager.h"
#include "CTransform.h"
#include "CInput.h"
#include "CCollider2D.h"


PlayerSelectButton::PlayerSelectButton()
	: Button(eUIType::Button)
{
}

PlayerSelectButton::~PlayerSelectButton()
{
	delete mCharterAnimation;
	mCharterAnimation = nullptr;
}

void PlayerSelectButton::Initalize()
{
	MeshRenderer* mr = this->AddComponent<MeshRenderer>();
	std::weak_ptr<Mesh> mesh = ResourceManager::GetInstance()->Find<Mesh>(L"RectMesh");
	std::weak_ptr<Material> material = ResourceManager::GetInstance()->Find<Material>(L"Rect2Material");

	mr->SetMesh(mesh);
	mr->SetMaterial(material);

	mCharterAnimation = new GameObject();
	mCharterAnimation->Initalize();

	Transform* tr = mCharterAnimation->GetComponent<Transform>();
	tr->SetScale(Vector3(0.5f, 0.5f, 0.0f));


	

	Transform* ButtonTr = GetComponent<Transform>();
	tr->SetParent(ButtonTr);

	Collider2D* col = mCharterAnimation->AddComponent<Collider2D>();
	col->SetOwner(mCharterAnimation);
	col->SetType(eColliderType::Rect);

	SpriteRenderer* sr = mCharterAnimation->AddComponent<SpriteRenderer>();
	//std::weak_ptr<Material> material = ResourceManager::GetInstance()->Find<Material>(L"Rect2Material");
	sr->SetMesh(mesh);
	sr->SetMaterial(material);

	/*Animator* animator = mCharterAnimation->AddComponent<Animator>();
	weak_ptr<Texture2D> tex = ResourceManager::GetInstance()->Find<Texture2D>(L"SorcericeSelect");
	animator->Create(L"CharterSelectAnimator", 
						tex, 
						Vector2::Zero, Vector2(0.f, 0.f), 
						Vector2::Zero, 16, 0.1f);*/

	

}

void PlayerSelectButton::Update()
{
	Button::Update();

	if (!mCharterAnimation)
		return;
	
	if (!mbPointToButton)
		return;

	if (Input::GetInstance()->GetKeyDown(eKeyCode::LBTN))
	{
		Click();
	}
}

void PlayerSelectButton::FixedUpdate()
{
	Button::FixedUpdate();

	if (!mCharterAnimation)
		return;

	mCharterAnimation->FixedUpdate();
}

void PlayerSelectButton::Render()
{
	Button::Render();

	if (!mCharterAnimation)
		return;

	mCharterAnimation->Render();
}
