#include "CPlayerSelectButton.h"
#include "CGameObject.h"
#include "CSpriteRenderer.h"
#include "CAnimator.h"
#include "CResourceManager.h"
#include "CTransform.h"
#include "CInput.h"
#include "CCollider2D.h"
#include "CSelectButtonSystem.h"


PlayerSelectButton::PlayerSelectButton()
	: Button(eUIType::Button)
	, mSystem(nullptr)
	, mChartType((UINT)eCharType::End)
	, mButtonIndex(-1)
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
	std::weak_ptr<Texture2D> tex = ResourceManager::GetInstance()->Load<Texture2D>(L"SelectUi", L"UI//CharterSelectUi.png");
	material.lock()->SetTexture(eTextureSlot::T0, tex);

	mr->SetMesh(mesh);
	mr->SetMaterial(material);

	Transform* ButtonTr = GetComponent<Transform>();
	//ButtonTr->SetPosition(Vector3(1.0f, 1.0f, 0.0f));



	// 캐릭터 선택 ui 초기화
	mCharterAnimation = new GameObject();
	mCharterAnimation->Initalize();

	Transform* tr = mCharterAnimation->GetComponent<Transform>();
	tr->SetScale(Vector3(0.6f, 1.2f, 1.0f));
	//tr->SetRotation(Vector3(30.f, 0.f, 0.f));
	tr->SetParent(ButtonTr);
	tr->SetPosition(Vector3(-0.8f, -0.0f, 0.0f));

	SpriteRenderer* sr = mCharterAnimation->AddComponent<SpriteRenderer>();
	std::weak_ptr<Material> selectMaterial = ResourceManager::GetInstance()->Find<Material>(L"ChaterSelectMaterial");

	sr->SetMesh(mesh);
	sr->SetMaterial(selectMaterial);


	Animator* animator = mCharterAnimation->AddComponent<Animator>();
	{
		std::shared_ptr<Texture2D> texture = std::make_shared<Texture2D>();
		texture->Load(L"Diablo2_Walk.png");
		ResourceManager::GetInstance()->Insert(L"PlayerWalk", texture);

		float x = 60.f;
		float y = 75.875f;

		for (int i = 0; i < 16; ++i)
		{
			wstring name = L"Walk";
			name += std::to_wstring(i);

			animator->Create(name, texture, Vector2(0.0f, y * i), Vector2(x, y), Vector2(0.0f, 0.0f), 8, 0.1f);
		}
		//60 75.875
		animator->Play(L"Walk0");
	}

}

void PlayerSelectButton::Update()
{
	Button::Update();

	if (!mCharterAnimation)
		return;
	
	if (mbPointToButton == 0 && mSystem->GetClickButton() != this)
	{
		BaseRenderer* ren = GetComponent<BaseRenderer>();
		ren->SetRenderStop(true);

		return;
	}

	BaseRenderer* ren = GetComponent<BaseRenderer>();
	ren->SetRenderStop(false);

	if (Input::GetInstance()->GetKeyDown(eKeyCode::LBTN))
	{
		Click();
		mSystem->SetClickButton(this);
	}

	mCharterAnimation->Update();
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
