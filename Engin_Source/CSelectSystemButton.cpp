#include "CSelectSystemButton.h"
#include "CInput.h"

SelectSystemButton::SelectSystemButton()
	: Button(eUIType::Button)
{
	mButtonTex.resize(2);
}

SelectSystemButton::~SelectSystemButton()
{
}

void SelectSystemButton::Initalize()
{
	Button::Initalize();
}

void SelectSystemButton::Update()
{
	Button::Update();

	BaseRenderer* ren = GetComponent<BaseRenderer>();
	if (ren == nullptr)
		return;

	if (mbPointToButton > 0 && Input::GetInstance()->GetKeyUp(eKeyCode::LBTN))
	{
		OnEvent();
	}

	if (mbPointToButton > 0 && Input::GetInstance()->GetKeyPress(eKeyCode::LBTN))
	{
		ren->GetMaterial().lock()->SetTexture(eTextureSlot::T0, mButtonTex[1]);
	}
	else
	{
		ren->GetMaterial().lock()->SetTexture(eTextureSlot::T0, mButtonTex[0]);
	}
}

void SelectSystemButton::FixedUpdate()
{
	Button::FixedUpdate();
}

void SelectSystemButton::Render()
{
	Button::Render();
}

void SelectSystemButton::SetButtonTex(UINT slot, weak_ptr<Texture2D> tex)
{
	if (slot > 1)
		return;

	mButtonTex[slot] = tex;
}
