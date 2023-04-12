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

void Button::OnTick()
{
	Vector2 mousePos = Input::GetInstance()->GetMousePos();
	Vector2 size = GetSize();

	if (mScreenPos.x <= mousePos.x && mousePos.x < mScreenPos.x + size.x
		&& mScreenPos.y <= mousePos.y && mousePos.y < mScreenPos.y + size.y)
	{
		mbMouseOn = true;
	}
	else
	{
		mbMouseOn = false;
	}

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


void Button::Click()
{
	int a = 0;
}
