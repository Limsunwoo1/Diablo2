#include "CSkilTreePanel.h"
#include "CButton.h"
#include "CSKiltreeButton.h"
#include "CResourceManager.h"
#include "CSpriteRenderer.h"


SkilTreePanel::SkilTreePanel()
	: UiBase(eUIType::Panel)
	, mHavePoint(0)
{
	ButtonList[0] = new Button(eUIType::Button);
	ButtonList[1] = new Button(eUIType::Button);
	ButtonList[2] = new Button(eUIType::Button);

	ButtonList[0]->BindEvnet(std::bind(&SkilTreePanel::ButtonSelect1, this));
	ButtonList[1]->BindEvnet(std::bind(&SkilTreePanel::ButtonSelect2, this));
	ButtonList[2]->BindEvnet(std::bind(&SkilTreePanel::ButtonSelect3, this));

	ButtonList[0]->GetComponent<Transform>()->SetPosition(Vector3(705.f, 155.f, 1.0f));
	ButtonList[1]->GetComponent<Transform>()->SetPosition(Vector3(705.f, -45.f, 1.0f));
	ButtonList[2]->GetComponent<Transform>()->SetPosition(Vector3(705.f, -235.f, 1.0f));

	ButtonList[0]->GetComponent<Transform>()->SetSize(Vector3(140.f, 170.f, 1.0f));
	ButtonList[1]->GetComponent<Transform>()->SetSize(Vector3(140.f, 170.f, 1.0f));
	ButtonList[2]->GetComponent<Transform>()->SetSize(Vector3(140.f, 170.f, 1.0f));

	mSkilTreeTex.resize(3);

	mSkilTreeTex[0] = ResourceManager::GetInstance()->Load<Texture2D>(L"FrozenSkilTree", L"..//Resource//UI//FrozenSkilTree.png");
	mSkilTreeTex[1] = ResourceManager::GetInstance()->Load<Texture2D>(L"LightSkilTree", L"..//Resource//UI//LightSkilTree.png");
	mSkilTreeTex[2] = ResourceManager::GetInstance()->Load<Texture2D>(L"FireSkilTree", L"..//Resource//UI//FireSkilTree.png");

	mCurButton = ButtonList[0];

	mMaterial = std::make_shared<Material>();
	mMaterial->SetRenderingMode(graphics::eRenderingMode::Transparent);

	mMaterial->SetTexture(eTextureSlot::T0, mSkilTreeTex[0]);

	std::weak_ptr<Mesh> mesh = ResourceManager::GetInstance()->Find<Mesh>(L"RectMesh");
	std::weak_ptr<Shader> shader = ResourceManager::GetInstance()->Find<Shader>(L"SpriteShader");

	mMaterial->SetShader(shader);

	SpriteRenderer* sr = AddComponent<SpriteRenderer>();
	sr->SetMesh(mesh);
	sr->SetMaterial(mMaterial);
}

SkilTreePanel::~SkilTreePanel()
{
	delete ButtonList[0];
	delete ButtonList[1];
	delete ButtonList[2];

	ButtonList[0] = nullptr;
	ButtonList[1] = nullptr;
	ButtonList[2] = nullptr;

	delete mFont;
	mFont = nullptr;
}

void SkilTreePanel::Initalize()
{
	mFont = new FontHUD();
	mFont->Initalize();
	mFont->SetCurNum(mHavePoint);

	Transform* fontTr = mFont->GetComponent<Transform>();
	fontTr->SetSize(Vector3(30.f, 30.f, 1.0f));
	fontTr->SetPosition(Vector3(705.f, 326.f, 1.0f));

	/// //////////////////////////////////////////////////////////////////
	AudioSource* audio = AddComponent<AudioSource>();
	std::weak_ptr<AudioClip>clip = ResourceManager::GetInstance()->Load<AudioClip>(L"UiOn", L"SoundResource\\scroll.wav");
	audio->SetClip(clip);
	audio->SetLoop(false);

	//////////////////////////////////////////////////////////////////////////////////////////////////
	SkiltreeButton* fireBolt = new SkiltreeButton();	fireBolt->SetSkilType(eSkilList::FireBolt);
	fireBolt->GetComponent<Transform>()->SetPosition(Vector3(400.f, 379.f, 1.0f));

	SkiltreeButton* lceBolt = new SkiltreeButton();		lceBolt->SetSkilType(eSkilList::FrozenBolt);
	lceBolt->GetComponent<Transform>()->SetPosition(Vector3(400.f, 385.f, 1.0f));

	SkiltreeButton* lightBolt = new SkiltreeButton();	lightBolt->SetSkilType(eSkilList::LightBolt);
	lightBolt->GetComponent<Transform>()->SetPosition(Vector3(400.f, 379.f, 1.0f));

	SkiltreeButton* telePort = new SkiltreeButton();	telePort->SetSkilType(eSkilList::TelePort);
	telePort->GetComponent<Transform>()->SetPosition(Vector3(400.f, 131.f, 1.0f));

	SkiltreeButton* FrozenOrb = new SkiltreeButton();	FrozenOrb->SetSkilType(eSkilList::FrozenOrb);
	FrozenOrb->GetComponent<Transform>()->SetPosition(Vector3(400.f, 2.f, 1.0f));

	SkiltreeButton* Meteor = new SkiltreeButton();		Meteor->SetSkilType(eSkilList::Meteor);
	Meteor->GetComponent<Transform>()->SetPosition(Vector3(400.f, 131.f, 1.0f));

	SkiltreeButton* FrozenArmer = new SkiltreeButton();	FrozenArmer->SetSkilType(eSkilList::FrozenArmer);
	FrozenArmer->GetComponent<Transform>()->SetPosition(Vector3(400.f, 255.f, 1.0f));


	ButtonList[0]->SetChild(lceBolt);
	ButtonList[1]->SetChild(lightBolt);
	ButtonList[2]->SetChild(fireBolt);

	// 화속성 계열 스킬연결
	fireBolt->SetMaxPoint(5);
	fireBolt->SetChild(Meteor);

	// 얼음 계열 스킬 연결
	lceBolt->SetMaxPoint(5);
	lceBolt->SetChild(FrozenArmer);

	FrozenArmer->SetMaxPoint(5);
	FrozenArmer->SetChild(FrozenOrb);

	// 전기 계열 스킬연결
	lightBolt->SetMaxPoint(5);
	lightBolt->SetChild(telePort);

	////////////////////////////////
	// 스킬버튼들 이미지
	wstring path = L"..//Resource//UI//";

	fireBolt->InitTexture(ResourceManager::GetInstance()->Load<Texture2D>(L"FirBolt", path + L"FireBoltButton.png"));
	lceBolt->InitTexture(ResourceManager::GetInstance()->Load<Texture2D>(L"FrozenBoltButton", path + L"FrozenBoltButton.png"));
	lightBolt->InitTexture(ResourceManager::GetInstance()->Load<Texture2D>(L"LightingBoltButton", path + L"LightingBoltButton.png"));
	telePort->InitTexture(ResourceManager::GetInstance()->Load<Texture2D>(L"TelePortButton", path + L"TelePortButton.png"));
	FrozenOrb->InitTexture(ResourceManager::GetInstance()->Load<Texture2D>(L"FrozenOrbButton", path + L"FrozenOrbButton.png"));
	Meteor->InitTexture(ResourceManager::GetInstance()->Load<Texture2D>(L"MeteorButton", path + L"MeteorButton.png"));
	FrozenArmer->InitTexture(ResourceManager::GetInstance()->Load<Texture2D>(L"FrozenArmerButton", path + L"FrozenArmer1.png"));
}

void SkilTreePanel::Update()
{
	if (mbEnable == false)
		return;

	GameObject::Update();

	if (mCurButton)
	{
		mCurButton->SetChildRun(true);
	}

	for (int i = 0; i < 3; ++i)
	{
		ButtonList[i]->Update();
	}

	mFont->Update();
}

void SkilTreePanel::FixedUpdate()
{
	if (mbEnable == false)
		return;

	GameObject::FixedUpdate();
	if (GetState() != eState::active)
		return;

	for (int i = 0; i < 3; ++i)
	{
		ButtonList[i]->FixedUpdate();
	}

	mFont->FixedUpdate();
}

void SkilTreePanel::Render()
{
	if (mbEnable == false)
		return;

	GameObject::Render();

	for (int i = 0; i < 3; ++i)
	{
		ButtonList[i]->Render();
	}

	mFont->Render();
}

void SkilTreePanel::OnActive()
{
	mbEnable = true;
	for (int i = 0; i < 3; ++i)
	{
		ButtonList[i]->OnActive();
	}

	mFont->OnActive();
}

void SkilTreePanel::UnActive()
{
	mbEnable = false;
	for (int i = 0; i < 3; ++i)
	{
		ButtonList[i]->UnActive();
	}

	mFont->UnActive();
}


void SkilTreePanel::ButtonSelect1()
{
	mCurButton = ButtonList[0];

	ButtonList[1]->SetChildRun(false);
	ButtonList[2]->SetChildRun(false);

	mMaterial->SetTexture(eTextureSlot::T0, mSkilTreeTex[0]);
}

void SkilTreePanel::ButtonSelect2()
{
	mCurButton = ButtonList[1];

	ButtonList[0]->SetChildRun(false);
	ButtonList[2]->SetChildRun(false);

	mMaterial->SetTexture(eTextureSlot::T0, mSkilTreeTex[1]);
}

void SkilTreePanel::ButtonSelect3()
{
	mCurButton = ButtonList[2];

	ButtonList[0]->SetChildRun(false);
	ButtonList[1]->SetChildRun(false);

	mMaterial->SetTexture(eTextureSlot::T0, mSkilTreeTex[2]);
}

std::vector<Button*> SkilTreePanel::GetButtonChild(UINT Idx)
{
	std::vector<Button*> returnButtons = {};
	Button* curButton = ButtonList[Idx];
	while (1)
	{
		Button* InputButton = curButton->GetChild();

		if (InputButton == nullptr)
			break;


		returnButtons.emplace_back(InputButton);
		curButton = InputButton;

		SkiltreeButton* skil = dynamic_cast<SkiltreeButton*>(curButton);
		if (skil != nullptr)
		{
			if (skil->GetPoint() < skil->GetMaxPoint())
				break;
		}
	}

	return returnButtons;
}

std::vector<eSkilList> SkilTreePanel::GetButtonChildType(UINT Idx)
{
	std::vector<eSkilList> OutSkilList = {};
	std::vector<Button*> returnButtons = {};
	Button* curButton = ButtonList[Idx];
	while (1)
	{
		Button* InputButton = curButton->GetChild();

		if (InputButton == nullptr)
			break;

		returnButtons.emplace_back(InputButton);
		curButton = InputButton;
	}

	for (Button* bot : returnButtons)
	{
		SkiltreeButton* skil = dynamic_cast<SkiltreeButton*>(bot);
		if (skil != nullptr)
		{
			OutSkilList.emplace_back(skil->GetSkilType());

			if (skil->GetPoint() < skil->GetMaxPoint())
				break;
		}
	}

	return OutSkilList;
}

void SkilTreePanel::SetPoint(UINT point)
{
	mHavePoint = point;
	mFont->SetCurNum(point);
}