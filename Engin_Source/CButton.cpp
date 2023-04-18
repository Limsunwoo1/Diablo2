#include "CButton.h"
#include "CInput.h"
#include "CTransform.h"
#include "CCollider2D.h"


Button::Button(eUIType type)
	: UiBase(type)
	, mbMouseOn(false)
{
	/*mOnClick = std::bind(&Button::Click, this);

	Collider2D* col = this->AddComponent<Collider2D>();
	col->SetType(eColliderType::Rect);*/
}
Button::~Button()
{

}

void Button::OnInit()
{
}

void Button::OnActive()
{
	mbEnable = true;
}

void Button::OnInActive()
{
}

void Button::OnUpdate()
{
	Vector2 mousePos = Input::GetInstance()->GetMouseWorldPos(false);

	Transform* tr = GetComponent<Transform>();
	Vector3 colPos = tr->GetPosition();
	Vector3 colScale = tr->GetScale();

	if (mousePos.x > colPos.x + (colScale.x * 0.5f) || mousePos.x < colPos.x - (colScale.x * 0.5f))
		return;
	if (mousePos.y < colPos.y + (colScale.x * 0.5f) || mousePos.y > colPos.y - (colScale.y * 0.5f))
		return;

	std::cout << " 충돌 " << std::endl;

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
	mOnClick = std::bind(&Button::Click, this);

	Collider2D* col = this->AddComponent<Collider2D>();
	col->SetType(eColliderType::Rect);
}

void Button::Update()
{
	static int count = 0;

	Vector2 mousePos = Input::GetInstance()->GetMouseWorldPos(false);

	Transform* tr = GetComponent<Transform>();
	float x, y, z;
	x = tr->GetWorldMatrix()._41;
	y = tr->GetWorldMatrix()._42;
	z = tr->GetWorldMatrix()._43;
	Vector3 colPos{ x,y,z };
	Vector3 colScale = tr->GetScale();

	if (mousePos.x > colPos.x + (colScale.x * 0.5f) || mousePos.x < colPos.x - (colScale.x * 0.5f))
		return;
	if (mousePos.y > colPos.y + (colScale.y * 0.5f) || mousePos.y < colPos.y - (colScale.y * 0.5f))
		return;

	std::cout << " 충돌 " << count++ << std::endl;
}

void Button::FixedUpdate()
{
	UiBase::FixedUpdate();
}

void Button::Render()
{
	UiBase::Render();
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
