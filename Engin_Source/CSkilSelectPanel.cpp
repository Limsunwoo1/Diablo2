#include "CSkilSelectPanel.h"
#include "CSkilSelectButton.h"
#include "CUIManager.h"
#include "CSkilTreePanel.h"
#include "CSKiltreeButton.h"
#include "CSpriteRenderer.h"
#include "CResourceManager.h"
#include "CFontHUD.h"

#include "CAudioSource.h"
#include "CAudioClip.h"

SkilSelectPanel::SkilSelectPanel()
	: UiBase(eUIType::Panel)
{
	mSelectButton[0].resize(5);
	mSelectButton[1].resize(5);
	mSelectButton[2].resize(5);
}

SkilSelectPanel::~SkilSelectPanel()
{
	for (int i = 0; i < mSelectButton[0].size(); ++i)
	{
		delete mSelectButton[0][i];
	}

	for (int i = 0; i < mSelectButton[1].size(); ++i)
	{
		delete mSelectButton[1][i];
	}

	for (int i = 0; i < mSelectButton[1].size(); ++i)
	{
		delete mSelectButton[2][i];
	}

}

void SkilSelectPanel::Initalize()
{
	///////////////////////////////////////////////////////////
	SpriteRenderer* sr = AddComponent<SpriteRenderer>();
	
	mMater = std::make_shared<Material>();
	mMater->SetRenderingMode(graphics::eRenderingMode::Transparent);
	sr->SetMaterial(mMater);
	sr->SetRenderStop(true);


	int interver = 100.f;
	for (int i = 0; i < mSelectButton[0].size(); ++i)
	{
		mSelectButton[0][i] = new SkilSelectButton();
		Transform* tr = mSelectButton[0][i]->GetComponent<Transform>();

		tr->SetPosition(Vector3(380.f -(interver * i), -270.f, 1.0f));
		tr->SetSize(Vector3(100.f, 100.f, 1.0f));
	}

	for (int i = 0; i < mSelectButton[1].size(); ++i)
	{
		mSelectButton[1][i] = new SkilSelectButton();
		Transform* tr = mSelectButton[1][i]->GetComponent<Transform>();

		tr->SetPosition(Vector3(380.f -(interver * i), -160.f, 1.0f));
		tr->SetSize(Vector3(100.f, 100.f, 1.0f));
	}

	for (int i = 0; i < mSelectButton[2].size(); ++i)
	{
		mSelectButton[2][i] = new SkilSelectButton();
		Transform* tr = mSelectButton[2][i]->GetComponent<Transform>();

		tr->SetPosition(Vector3(380.f  -(interver * i), -50.f, 1.0f));
		tr->SetSize(Vector3(100.f, 100.f, 1.0f));
	}
}

void SkilSelectPanel::Update()
{
	if (mbEnable == false)
		return;

	for (int i = 0; i < mSelectButton[0].size(); ++i)
	{
		mSelectButton[0][i]->Update();
	}

	for (int i = 0; i < mSelectButton[1].size(); ++i)
	{
		mSelectButton[1][i]->Update();
	}

	for (int i = 0; i < mSelectButton[2].size(); ++i)
	{
		mSelectButton[2][i]->Update();
	}
}

void SkilSelectPanel::FixedUpdate()
{
	if (mbEnable == false)
		return;

	for (int i = 0; i < mSelectButton[0].size(); ++i)
	{
		mSelectButton[0][i]->FixedUpdate();
	}

	for (int i = 0; i < mSelectButton[1].size(); ++i)
	{
		mSelectButton[1][i]->FixedUpdate();
	}

	for (int i = 0; i < mSelectButton[2].size(); ++i)
	{
		mSelectButton[2][i]->FixedUpdate();
	}
}

void SkilSelectPanel::Render()
{
	if (mbEnable == false)
		return;

	for (int i = 0; i < mSelectButton[0].size(); ++i)
	{
		mSelectButton[0][i]->Render();
	}

	for (int i = 0; i < mSelectButton[1].size(); ++i)
	{
		mSelectButton[1][i]->Render();
	}

	for (int i = 0; i < mSelectButton[2].size(); ++i)
	{
		mSelectButton[2][i]->Render();
	}
}

void SkilSelectPanel::OnActive()
{
	mbEnable = true;
	ResetTex();

	for (int i = 0; i < mSelectButton[0].size(); ++i)
	{
		mSelectButton[0][i]->OnActive();
	}

	for (int i = 0; i < mSelectButton[1].size(); ++i)
	{
		mSelectButton[1][i]->OnActive();
	}

	for (int i = 0; i < mSelectButton[2].size(); ++i)
	{
		mSelectButton[2][i]->OnActive();
	}
}

void SkilSelectPanel::UnActive()
{
	mbEnable = false;

	for (int i = 0; i < mSelectButton[0].size(); ++i)
	{
		mSelectButton[0][i]->UnActive();
	}

	for (int i = 0; i < mSelectButton[1].size(); ++i)
	{
		mSelectButton[1][i]->UnActive();
	}

	for (int i = 0; i < mSelectButton[2].size(); ++i)
	{
		mSelectButton[2][i]->UnActive();
	}
}

void SkilSelectPanel::ResetTex()
{
	SkilTreePanel* tree = UIManager::GetInstance()->GetUiInstance<SkilTreePanel>(L"SkilTree");
	std::vector<Button*> Buttons = tree->GetButtonChild(0);
	std::vector<eSkilList> TypeList = tree->GetButtonChildType(0);

	for (int i = 0; i < Buttons.size(); ++i)
	{
		SkiltreeButton* skil = dynamic_cast<SkiltreeButton*>(Buttons[i]);
		if (skil == nullptr)
			continue;

		std::weak_ptr<Texture2D> tex = skil->GetTex();
		mSelectButton[0][i]->GetMaterial().lock()->SetTexture(eTextureSlot::T0, tex);
		mSelectButton[0][i]->SetSkilType(skil->GetSkilType());
	}
	
	Buttons = tree->GetButtonChild(1);
	TypeList = tree->GetButtonChildType(1);
	for (int i = 0; i < Buttons.size(); ++i)
	{
		SkiltreeButton* skil = dynamic_cast<SkiltreeButton*>(Buttons[i]);
		if (skil == nullptr)
			continue;

		std::weak_ptr<Texture2D> tex = skil->GetTex();
		mSelectButton[1][i]->GetMaterial().lock()->SetTexture(eTextureSlot::T0, tex);
		mSelectButton[1][i]->SetSkilType(skil->GetSkilType());
	}

	Buttons = tree->GetButtonChild(2);
	TypeList = tree->GetButtonChildType(2);
	for (int i = 0; i < Buttons.size(); ++i)
	{
		SkiltreeButton* skil = dynamic_cast<SkiltreeButton*>(Buttons[i]);
		if (skil == nullptr)
			continue;

		std::weak_ptr<Texture2D> tex = skil->GetTex();
		mSelectButton[2][i]->GetMaterial().lock()->SetTexture(eTextureSlot::T0, tex);
		mSelectButton[2][i]->SetSkilType(skil->GetSkilType());
	}

}

