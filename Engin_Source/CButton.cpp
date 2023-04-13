#include "CButton.h"
#include "CInput.h"


Button::Button(eUIType type)
	: UiBase(type)
	, mbMouseOn(false)
{
	mOnClick = std::bind(&Button::Click, this);
}
Button::~Button()
{

}

void Button::OnInit()
{
}

void Button::OnActive()
{
}

void Button::OnInActive()
{
}

void Button::OnUpdate()
{
	Vector2 mousePos = Input::GetInstance()->GetMousePos();

	/*if (mScreenPos.x <= mousePos.x && mousePos.x < mScreenPos.x + size.x
		&& mScreenPos.y <= mousePos.y && mousePos.y < mScreenPos.y + size.y)
	{
		mbMouseOn = true;
	}
	else
	{
		mbMouseOn = false;
	}*/

	if (Input::GetInstance()->GetKeyDown(eKeyCode::LBTN) && mbMouseOn)
	{
		mOnClick();
	}
}

void Button::OnRender(HDC hdc)
{

}

void Button::OnClear()
{
}

void Button::Initalize()
{
}

void Button::Update()
{
}

void Button::FixedUpdate()
{
}

void Button::Render()
{
}

void Button::InitAnimation()
{
}

void Button::InitRenderer(const wstring& materialName, const wstring& textureName, const std::wstring& textruepath)
{
}


void Button::Click()
{
	int a = 0;
}
