#include "CInput.h"
#include "CApplication.h"
#include "CRenderer.h"

extern CApplication Application;

int ASCII[(UINT)eKeyCode::End] =
{
	'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P',
	'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'K',
	'Z', 'X', 'C', 'V', 'B', 'N', 'M',

	//		//Special Key
	//ENTER, ESC, LSHIFT, LALT, LCTRL,
	//SPACE, LEFT, RIGHT, UP, DOWN,
	//LBTN, RBTN,

	VK_RETURN, VK_ESCAPE, VK_LSHIFT, VK_LMENU, VK_LCONTROL,
	VK_SPACE, VK_LEFT, VK_RIGHT, VK_UP, VK_DOWN,
	VK_LBUTTON, VK_RBUTTON,

	////Num Pad
	VK_NUMPAD0, VK_NUMPAD1, VK_NUMPAD2,
	VK_NUMPAD3, VK_NUMPAD4, VK_NUMPAD5,
	VK_NUMPAD6, VK_NUMPAD7, VK_NUMPAD8,
	VK_NUMPAD9,


	////Num Line
	'0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
};

Input::Input()
{

}

Input::~Input()
{

}

void Input::Initialize()
{
	for (int i = 0; i < (UINT)eKeyCode::End; i++)
	{
		// ���� Ű ����
		// eKeyCode KeyCode;
		// Ű���� ����
		// eKeyState state;
		// bool bPressed;

		Key key;
		key.keyCode = (eKeyCode)i;
		key.state = eKeyState::NONE;
		key.bPressed = false;

		mKeys.push_back(key);
	}
}

void Input::Update()
{
	// ���콺�� ���� window â�� �����Ͽ����� üũ
	if (GetFocus())
	{
		// Ű����
		for (UINT i = 0; i < (UINT)eKeyCode::End; i++)
		{
			// �ش�Ű�� ���� ������ �ִ°��
			if (GetAsyncKeyState(ASCII[i]) & 0x8000)
			{
				// ���� �����ӿ� ������ �־���
				if (mKeys[i].bPressed)
					mKeys[i].state = eKeyState::PRESSED;
				else
					mKeys[i].state = eKeyState::DOWN;

				mKeys[i].bPressed = true;
			}
			// �ش�Ű�� ������ ���� ���� ���
			else
			{
				// ���� �����ӿ� ������ �־���
				if (mKeys[i].bPressed)
					mKeys[i].state = eKeyState::UP;
				else
					mKeys[i].state = eKeyState::NONE;

				mKeys[i].bPressed = false;
			}
		}
	}

	POINT mousePos = {};

	// ���� ���콺 ��ǥ�� �޾ƿ�
	GetCursorPos(&mousePos);

	// ���콺 �������� Ŭ���̾�Ʈ â�� ����
	/*HWND hwnd = Application::GetInstance().GetWindowData().hWnd;
	ScreenToClient(hwnd, &mousePos);*/


	mMousePos.x = (float)mousePos.x;
	mMousePos.y = (float)mousePos.y;
}

void Input::Render(HDC hdc)
{

}

eKeyState Input::GetkeyState(eKeyCode keyCode)
{
	return mKeys[(UINT)keyCode].state;
}

Math::Vector2 Input::GetMousePos(HWND hWnd)
{
	Math::Vector2 vMousePos(-1.0f, -1.0f);
	if (GetFocus())
	{
		POINT mousePos = {};
		GetCursorPos(&mousePos);
		ScreenToClient(hWnd, &mousePos);
		vMousePos.x = (float)mousePos.x;
		vMousePos.y = (float)mousePos.y;

		return vMousePos;
	}

	return vMousePos;
}

Math::Vector2 Input::GetMouseWorldPos(bool camType)
{
	Vector2 mouse = GetMousePos(Application.GetHwnd());
	Math::Viewport mathViewport;

	// viewport ũ��
	D3D11_VIEWPORT viewport;
	UINT numVIewPorts = 1;
	graphics::GetDevice()->GetID3D11DeviceContext()->RSGetViewports(&numVIewPorts, &viewport);

	mathViewport.x = viewport.TopLeftX;
	mathViewport.y = viewport.TopLeftY;
	mathViewport.width = viewport.Width;
	mathViewport.height = viewport.Height;
	mathViewport.minDepth = viewport.MinDepth;
	mathViewport.maxDepth = viewport.MaxDepth;

	// ��ũ�� ��ǥ�� NDC ��ǥ�� ��ȯ
	/*float ndcX = ((2.0f * mouse.x) / viewport.Width) - 1.0f;
	float ndcY = 1.0f - ((2.0f * mouse.y) / viewport.Height);*/

	float ndcX = (2.0f * mouse.x / viewport.Width - 1.0f);
	float ndcY = (-2.0f * mouse.y / viewport.Height + 1.0f);

	/*float ndcX = (2.0f * mouse.x);
	ndcX /= viewport.Width;
	ndcX -= 1.0f;
	float ndcY = (2.0f * mouse.y);
	ndcY /= viewport.Height;
	ndcY -= 1.0f;*/

	float ndxZ = 1.0f;

	// NDC ��ǥ�� ���� ��ǥ�� ��ȯ
	Math::Vector3 nearPoint(ndcX, ndcY, 0.0f);
	//Vector3 result;

	/*Math::Matrix projection = Renderer::mainCamera->GetProjectionMatrix().Invert();
	Math::Matrix view = Renderer::mainCamera->GetViewMatrix().Invert();*/
	Vector3 result = Vector3::Zero;
	if (camType)
	{
		Math::Matrix projection = Renderer::mainCamera->GetProjectionMatrix();
		Math::Matrix view = Renderer::mainCamera->GetViewMatrix();
		result = mathViewport.Unproject(Vector3(mouse.x, mouse.y, 1.0f), projection, view, Math::Matrix::Identity);
	}
	else
	{
		Math::Matrix projection = Renderer::UiCamera->GetProjectionMatrix();
		Math::Matrix view = Renderer::UiCamera->GetViewMatrix();
		result = mathViewport.Unproject(Vector3(mouse.x, mouse.y, 1.0f), projection, view, Math::Matrix::Identity);
	}

	/*Math::Vector3::Transform(nearPoint, projection, result);
	Math::Vector3::Transform(result, view, result);*/

	/*Math::Matrix projection1 = Renderer::mainCamera->GetProjectionMatrix();
	Math::Matrix view2 = Renderer::mainCamera->GetViewMatrix();*/

	/*Math::Matrix mat;
	mat = projection;
	mat *= view;*/

	return Math::Vector2(result.x, result.y);
}

Math::Vector2 Input::GetMouseScreenIndex()
{
	Vector2 mouse = GetMousePos();

	int x = mouse.x / 100;
	int y = mouse.y / 100;

	x -= 8;
	y -= 4;
	//12 7
	// �߾� �÷��̾� �ε���

	return Math::Vector2(x, y);
}
