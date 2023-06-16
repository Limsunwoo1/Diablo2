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
	tr->SetSize(Vector3(300.f, 200.f, 1.0f));
	//tr->SetRotation(Vector3(30.f, 0.f, 0.f));
	tr->SetParent(ButtonTr);
	tr->SetPosition(Vector3(-0.8f * 100, -0.0f, 0.0f));

	SpriteRenderer* sr = mCharterAnimation->AddComponent<SpriteRenderer>();
	std::weak_ptr<Material> selectMaterial = ResourceManager::GetInstance()->Find<Material>(L"ChaterSelectMaterial");

	sr->SetMesh(mesh);
	sr->SetMaterial(selectMaterial);


	Animator* animator = mCharterAnimation->AddComponent<Animator>();
	{
		std::weak_ptr<Texture2D> texture = ResourceManager::GetInstance()->Find<Texture2D>(L"PlayerWalk");

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

	// Run
	{
		std::weak_ptr<Texture2D> texture = ResourceManager::GetInstance()->Find<Texture2D>(L"PlayerRun");
		/*animator->Create(L"Walk0", texture, Vector2(0.0f, 0.0f), Vector2(60.f, 75.875f), Vector2(0.0f, 0.0f), 8, 0.1f);
		animator->Create(L"WalkUp", texture, Vector2(0.0f, 75.875 * 8), Vector2(60.f, 75.875f), Vector2(0.0f, 0.0f), 8, 0.1f);*/

		float x = 61.f;
		float y = 75.625f;

		for (int i = 0; i < 16; ++i)
		{
			wstring name = L"Run";
			name += std::to_wstring(i);

			animator->Create(name, texture, Vector2(0.0f, y * i), Vector2(x, y), Vector2(0.0f, 0.0f), 8, 0.1f);
		}
		//61 75.625
	}

	{
		std::weak_ptr<Texture2D> texture = ResourceManager::GetInstance()->Find<Texture2D>(L"PlayerIdle");

		float x = 66.f;
		float y = 72.f;

		for (int i = 0; i < 16; ++i)
		{
			wstring name = L"Idle";
			name += std::to_wstring(i);

			animator->Create(name, texture, Vector2(0.0f, y * i), Vector2(x, y), Vector2(0.0f, 0.0f), 8, 0.1f);
		}

		// 66  72
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

		
		wstring& name = mCharterAnimation->GetComponent<Animator>()->GetPlayAnimation()->AnimationName();
		if(name != L"Idle0")
			mCharterAnimation->GetComponent<Animator>()->Play(L"Idle0", false);

		return;
	}

	if (mbPointToButton > 0 && mSystem->GetClickButton() != this)
	{
		wstring& name = mCharterAnimation->GetComponent<Animator>()->GetPlayAnimation()->AnimationName();
		if (name != L"Walk0")
			mCharterAnimation->GetComponent<Animator>()->Play(L"Walk0");
	}

	BaseRenderer* ren = GetComponent<BaseRenderer>();
	ren->SetRenderStop(false);

	if (mbPointToButton > 0 && Input::GetInstance()->GetKeyDown(eKeyCode::LBTN))
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

void PlayerSelectButton::Click()
{
	mCharterAnimation->GetComponent<Animator>()->Play(L"Run0");
}

void PlayerSelectButton::SetInfo(wstring& name, UINT type, UINT level, float hp, float mp, float exp, Vector3 pos)
{
	mInfo.CharterType = type;
	mInfo.Level = level;
	mInfo.Hp = hp;
	mInfo.Mp = mp;
	mInfo.Exp = exp;

	mInfo.Name = name;
}
