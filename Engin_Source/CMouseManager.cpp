#include "CMouseManager.h"

MouseManager::MouseManager()
{

}

MouseManager::~MouseManager()
{

}

void MouseManager::Update()
{
	//if (GetFocus())
	//{
	//	// 키보드
	//	for (UINT i = 0; i < (UINT)eKeyCode::End; i++)
	//	{
	//		// 해당키가 현재 눌려져 있는경우
	//		if (GetAsyncKeyState(ASCII[i]) & 0x8000)
	//		{
	//			// 이전 프레임에 눌려져 있었다
	//			if (mKeys[i].bPressed)
	//				mKeys[i].state = eKeyState::PRESSED;
	//			else
	//				mKeys[i].state = eKeyState::DOWN;

	//			mKeys[i].bPressed = true;
	//		}
	//		// 해당키가 눌려져 있지 않은 경우
	//		else
	//		{
	//			// 이전 프레임에 눌려져 있었다
	//			if (mKeys[i].bPressed)
	//				mKeys[i].state = eKeyState::UP;
	//			else
	//				mKeys[i].state = eKeyState::NONE;

	//			mKeys[i].bPressed = false;
	//		}
	//	}
	//}
}
