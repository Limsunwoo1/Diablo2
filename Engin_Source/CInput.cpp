#include "CInput.h"
#include "CApplication.h"
#include "CRenderer.h"
#include "CTransform.h"
#include "CItemBase.h"

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
	: mIsometricTileIndexX(-1)
	, mIsometricTileIndexY(-1)
{

}

Input::~Input()
{

}

void Input::Initialize()
{
	for (int i = 0; i < (UINT)eKeyCode::End; i++)
	{
		// 나의 키 종류
		// eKeyCode KeyCode;
		// 키보드 상태
		// eKeyState state;
		// bool bPressed;

		Key key;
		key.keyCode = (eKeyCode)i;
		key.state = eKeyState::NONE;
		key.bPressed = false;

		mKeys.push_back(key);
	}

	mbMouseItemPick = false;
	mPickItem = nullptr;
}

void Input::Update()
{
	// 마우스가 현재 window 창을 선택하였는지 체크
	if (GetFocus())
	{
		// 키보드
		for (UINT i = 0; i < (UINT)eKeyCode::End; i++)
		{
			// 해당키가 현재 눌려져 있는경우
			if (GetAsyncKeyState(ASCII[i]) & 0x8000)
			{
				// 이전 프레임에 눌려져 있었다
				if (mKeys[i].bPressed)
					mKeys[i].state = eKeyState::PRESSED;
				else
					mKeys[i].state = eKeyState::DOWN;

				mKeys[i].bPressed = true;
			}
			// 해당키가 눌려져 있지 않은 경우
			else
			{
				// 이전 프레임에 눌려져 있었다
				if (mKeys[i].bPressed)
					mKeys[i].state = eKeyState::UP;
				else
					mKeys[i].state = eKeyState::NONE;

				mKeys[i].bPressed = false;
			}
		}
	}

	POINT mousePos = {};

	// 현재 마우스 좌표를 받아옴
	GetCursorPos(&mousePos);

	// 마우스 포지션을 클라이언트 창에 맞춤
	/*HWND hwnd = Application::GetInstance().GetWindowData().hWnd;
	ScreenToClient(hwnd, &mousePos);*/

	mMousePos.x = (float)mousePos.x;
	mMousePos.y = (float)mousePos.y;

	if (mPickItem != nullptr)
	{
		Transform* itemTr = mPickItem->GetComponent<Transform>();
		Vector3 pos = itemTr->GetPosition();

		Vector2 mouse = GetMouseWorldPos(false);

		pos.x = mouse.x;
		pos.y = mouse.y;

		itemTr->SetPosition(pos);
	}
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

	// viewport 크기
	D3D11_VIEWPORT viewport;
	UINT numVIewPorts = 1;
	graphics::GetDevice()->GetID3D11DeviceContext()->RSGetViewports(&numVIewPorts, &viewport);

	mathViewport.x = viewport.TopLeftX;
	mathViewport.y = viewport.TopLeftY;
	mathViewport.width = viewport.Width;
	mathViewport.height = viewport.Height;
	mathViewport.minDepth = viewport.MinDepth;
	mathViewport.maxDepth = viewport.MaxDepth;

	float ndcX = (2.0f * mouse.x / viewport.Width - 1.0f);
	float ndcY = (-2.0f * mouse.y / viewport.Height + 1.0f);


	float ndxZ = 1.0f;

	// NDC 좌표를 월드 좌표로 변환
	Math::Vector3 nearPoint(ndcX, ndcY, 0.0f);

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

	return Math::Vector2(result.x, result.y);
}

Math::Vector2 Input::GetMouseWorldPos(Vector2 windowMouse)
{
	Vector2 mouse = windowMouse;
	Math::Viewport mathViewport;

	// viewport 크기
	D3D11_VIEWPORT viewport;
	UINT numVIewPorts = 1;
	graphics::GetDevice()->GetID3D11DeviceContext()->RSGetViewports(&numVIewPorts, &viewport);

	mathViewport.x = viewport.TopLeftX;
	mathViewport.y = viewport.TopLeftY;
	mathViewport.width = viewport.Width;
	mathViewport.height = viewport.Height;
	mathViewport.minDepth = viewport.MinDepth;
	mathViewport.maxDepth = viewport.MaxDepth;

	float ndcX = (2.0f * mouse.x / viewport.Width - 1.0f);
	float ndcY = (-2.0f * mouse.y / viewport.Height + 1.0f);


	float ndxZ = 1.0f;

	// NDC 좌표를 월드 좌표로 변환
	Math::Vector3 nearPoint(ndcX, ndcY, 0.0f);

	Vector3 result = Vector3::Zero;

	Math::Matrix projection = Renderer::mainCamera->GetProjectionMatrix();
	Math::Matrix view = Renderer::mainCamera->GetViewMatrix();
	result = mathViewport.Unproject(Vector3(mouse.x, mouse.y, 1.0f), projection, view, Math::Matrix::Identity);

	return Math::Vector2(result.x, result.y);
}

Math::Vector2 Input::GetMouseWorldPos(class Camera* camera)
{
	Vector2 mouse = GetMousePos(Application.GetHwnd());
	Math::Viewport mathViewport;

	// viewport 크기
	D3D11_VIEWPORT viewport;
	UINT numVIewPorts = 1;
	graphics::GetDevice()->GetID3D11DeviceContext()->RSGetViewports(&numVIewPorts, &viewport);

	mathViewport.x = viewport.TopLeftX;
	mathViewport.y = viewport.TopLeftY;
	mathViewport.width = viewport.Width;
	mathViewport.height = viewport.Height;
	mathViewport.minDepth = viewport.MinDepth;
	mathViewport.maxDepth = viewport.MaxDepth;

	float ndcX = (2.0f * mouse.x / viewport.Width - 1.0f);
	float ndcY = (-2.0f * mouse.y / viewport.Height + 1.0f);


	float ndxZ = 1.0f;

	// NDC 좌표를 월드 좌표로 변환
	Math::Vector3 nearPoint(ndcX, ndcY, 0.0f);

	Vector3 result = Vector3::Zero;

	Math::Matrix projection = camera->GetProjectionMatrix();
	Math::Matrix view = camera->GetViewMatrix();
	result = mathViewport.Unproject(Vector3(mouse.x, mouse.y, 1.0f), projection, view, Math::Matrix::Identity);

	return Math::Vector2(result.x, result.y);
}


Math::Vector2 Input::GetMouseScreenIndex()
{
	Vector2 mouse = GetMousePos();

	int x = mouse.x;
	int y = mouse.y;

	x -= 8;
	y -= 4;
	//12 7
	// 중앙 플레이어 인덱스

	return Math::Vector2(x, y);
}

//UINT CPlayerScript::PlayerPicking(bool bLeft)
//{
//	if (m_fAttackCoolTime > 0.f)
//		return 0;
//	else
//	{
//		m_fAttackCoolTime = PLAYER_ATTACK_COOLTIME;
//	}
//	//POINT vPoint = CKeyMgr::GetInst()->GetMousePos();
//
//	//tResolution vResolution = CRenderMgr::GetInst()->GetResolution();
//
//	//Vec2 vPoint = Vec2(vResolution.fWidth / 2.f, vResolution.fHeight / 2.f);
//
//	Matrix matProj = m_pMainCamera->GetProjMat();
//
//	Vec3 vDirRay, vPosRay;
//	vDirRay.x = ((2.0f * (float)vPoint.x) / vResolution.fWidth - 1.f) / matProj._11;
//	vDirRay.y = ((-2.0f * (float)vPoint.y) / vResolution.fHeight + 1.f) / matProj._22;
//	vDirRay.z = 1.f;
//
//	Matrix matView = m_pMainCamera->GetViewMat();
//	Matrix matViewInv = m_pMainCamera->GetViewMatInv();
//
//	vPosRay = XMVector3TransformCoord(vPosRay, matViewInv);
//	vDirRay = XMVector3TransformNormal(vDirRay, matViewInv);
//	vDirRay = XMVector3Normalize(vDirRay);
//
//	CGameObject* pCollider = NULL;
//	// 충돌반경 안에있는 물체들과 피킹을 수행
//	for (int i = 0; i < m_vCollisionObj.size(); ++i)
//	{
//		if (CollisionRay(vPosRay, vDirRay, m_vCollisionObj[i]->Collider2D()))
//		{
//			pCollider = m_vCollisionObj[i];
//			break;
//		}
//	}
//}
//
//bool CPlayerScript::CollisionRay(Vec3 vPosRay, Vec3 vDirRay, CCollider2D* _pOther)
//{
//	Matrix matWorldInv = XMMatrixIdentity();
//	Vec3 vPos = _pOther->Transform()->GetLocalPos();
//	vPos += _pOther->Collider2D()->GetOffsetPos();
//	matWorldInv._41 = -vPos.x;
//	matWorldInv._42 = -vPos.y;
//	matWorldInv._43 = -vPos.z;
//
//	vPosRay = XMVector3TransformCoord(vPosRay, matWorldInv);
//	vDirRay = XMVector3TransformNormal(vDirRay, matWorldInv);
//
//	Vec3 vv = XMVector3Dot(vDirRay, vDirRay);
//	Vec3 qv = XMVector3Dot(vPosRay, vDirRay);
//	Vec3 qq = XMVector3Dot(vPosRay, vPosRay);
//
//	float r;
//	// collider2D radius
//	float rr = r * r;
//
//	if (qv.x * qv.x - vv.x * (qq.x - rr) >= 0)
//	{
//		return true;
//	}
//	return false;
//}
